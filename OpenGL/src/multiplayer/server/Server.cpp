#include "Server.hpp"

#include <SFML/Config.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <glm/glm.hpp>
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <chrono>

#include "entity/Entity.hpp"
#include "multiplayer/Packet.hpp"
#include "multiplayer/Serialize.hpp"
#include "multiplayer/server/Client.hpp"
#include "save/SaveManager.hpp"
#include "save/ServerConfig.hpp"
#include "terrain/ChunkImpl.hpp"
#include "terrain/BlockArray.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"
#include "util/Identifier.hpp"
#include "debug/Debug.hpp"

using namespace glm;
using namespace serde;

Server::Server(unsigned short port)
  : port(port), world(World::getInst())
{
  auto& config = Config::getServerConfig();
  renderDistH = config.renderDistH;
  renderDistV = config.renderDistV;
  World::getInst().t = (uint32_t)(8.f * dayDuration / 24.f);
  auto now = std::chrono::steady_clock::now().time_since_epoch();
  lastClock = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

Server::~Server()
{}

void Server::broadcast(sf::Packet& packet) {
  for(auto const& pair : clients) {
    send(packet, pair.first);
  }
}

void Server::on_packet_recv(sf::Packet& packet, ClientID client) {
  std::lock_guard<std::mutex> lck(mutex);
  PacketHeader header;
  packet >> header;
  auto type = header.getType();

  if(type == PacketType::LOGIN) handle_login(client, packet);
  else if(type == PacketType::LOGOUT) handle_logout(client);

  else {
    auto it = clients.find(client);
    if(it == clients.end()) {
      spdlog::warn("Client not registered: {}", client.getAddr());
    }
    else {
      it->second.lastUpdate = clock.getElapsedTime();
      if(type == PacketType::PING)               handle_ping(it->second);
      else if(type == PacketType::BLOCKS)        handle_blocks(it->second, packet);
      else if(type == PacketType::PLAYER_TICK)   handle_player_tick(it->second, packet);
      else if(type == PacketType::PLAYER_ACTION) handle_player_action(it->second, packet);
      else if(type == PacketType::CHUNKS)        handle_chunks(it->second, packet);
      else if(type == PacketType::ACK_CHUNKS)    handle_ack_chunks(it->second, packet);
    }
  }
}

void Server::on_server_tick() {
  auto now = std::chrono::steady_clock::now().time_since_epoch();
  uint32_t time = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
  World::getInst().dt = time - lastClock; 
  World::getInst().t += World::getInst().dt;
  World::getInst().t = World::getInst().t % dayDuration;
  lastClock = time;

  std::lock_guard<std::mutex> lck(mutex);
  packet_entity_tick();
  packet_chunks();
  remOldChunks();
  handleTimeouts();
}

void Server::packet_entity_tick() {
  sf::Packet packet;
  PacketHeader header(PacketType::ENTITY_TICK);
  packet << header;
  packet << (sf::Uint64)clients.size();

  for(auto const& pair : clients) {
    packet << pair.second.uid;
    packet << pair.second.player;
  }

  auto curTime = clock.getElapsedTime();

  for(auto& pair : clients) {
    if(pair.second.ack || curTime - pair.second.lastUpdate > tickAckLimit) {
      send(packet, pair.first);
      pair.second.ack = false;
    }
  }
}

void Server::beep() {
  std::cout << '\a';
  std::cout.flush();
}

void Server::packet_logout(Identifier uid) {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGOUT);
  packet << header << uid;
  broadcast(packet);
}

void Server::packet_blocks(Identifier uid, BlockArray changedBlocks) {
  sf::Packet packet;
  PacketHeader header(PacketType::BLOCKS);
  packet << header << uid << changedBlocks;
  broadcast(packet);
}

void Server::packet_ack_login(ClientID client, Identifier uid) {
  sf::Packet packet;
  PacketHeader header(PacketType::ACK_LOGIN);
  packet << header << world.t;
  send(packet, client);
}

void Server::packet_chunks() {
  static const size_t maxChunks = 5;

  for(auto& pair : clients) {
    auto& client = pair.second;

    std::vector<std::shared_ptr<Chunk>> chunks;
    for(auto it = client.waitingChunks.begin(); it != client.waitingChunks.end() && chunks.size() < maxChunks; ) {
      ivec3 cpos = *it;
      auto chunk = world.chunks.find(cpos);

      if(chunk && chunk->isComputed()) {
        chunks.push_back(chunk);
        it = client.waitingChunks.erase(it);
      }
      else ++it;
    }

    if(chunks.size() > 0) {
      sf::Packet packet;
      PacketHeader header(PacketType::CHUNKS);
      packet << header << (sf::Uint8)chunks.size();

      for(auto const& chunk : chunks) {
        packet << (sf::Uint8)chunk->size().x << chunk->chunkPos << *chunk;
      }

      send(packet, pair.first);
    }
  }
}

void Server::handle_login(ClientID client, sf::Packet& packet) {
  Identifier uid;
  packet >> uid;

  auto it = clients.find(client);

  if(it != clients.end()) {
    spdlog::warn("Login packet of already logged client: {}", client.getAddr());
    packet_ack_login(it->first, it->second.uid);
  }
  else {
    auto res = clients.emplace(client, Client(uid, clock.getElapsedTime()));

    if(res.second) {
      packet_ack_login(res.first->first, uid);
      beep();
      spdlog::info("Client connected: uid {} / addr {}", res.first->second.uid, res.first->first.getAddr());
    }
    else {
      spdlog::warn("Client insertion failed");
    }
  }
}

void Server::handle_logout(ClientID client) {
  auto it = clients.find(client);

  if(it == clients.end()) {
    spdlog::warn("Logout of unregistered client: {}", client.getAddr());
  }
  else {
    Identifier uid = it->second.uid;
    clients.erase(it);
    packet_logout(uid);
    beep();
    spdlog::info("Client disconnected: {}", uid);
  }
}

void Server::handle_ping(Client& client) {
  spdlog::info("Ping! from {}", client.uid);
  beep();
}

void Server::handle_player_tick(Client& client, sf::Packet& packet) {
  packet >> client.player;
  client.ack = true;
}

void Server::handle_player_action(Client& client, sf::Packet& packet) {
  PacketHeader header(PacketType::PLAYER_ACTION);
  sf::Packet pck;
  Action action;
  packet >> action;
  pck << header << client.uid << action;
  for(auto& pair : clients) {
    if(pair.second.uid != client.uid)
      send(pck, pair.first);    
  }
}

void Server::handle_chunks(Client& client, sf::Packet& packet) {
  packet >> client.waitingChunks;

  updateWaitingChunks();
}

void Server::handle_ack_chunks(Client& client, sf::Packet& packet) {
  std::vector<ivec3> ack;
  packet >> ack;

  for(auto const& cpos : ack) {
    auto it = std::find(client.waitingChunks.begin(), client.waitingChunks.end(), cpos);
    if(it != client.waitingChunks.end()) client.waitingChunks.erase(it);
  }
}

void Server::handle_blocks(Client const& client, sf::Packet& packet) {
  BlockArray blocks;
  packet >> blocks;
  blocks.copyToWorld();

  for(auto cpos : blocks.getChangedChunks()) {
    auto chunk = world.chunks.find(cpos);
    if(chunk) {
      SaveManager::saveChunk(*chunk);
      
      // need to also save all chunks in which the chunk has generated a slice.
      for(ivec3 cpos : chunk->slices)  {
        auto sliceChunk = world.chunks.find(cpos);
        if(!sliceChunk) {
          spdlog::warn("TODO: chunk {}, containing a slice from {}, is unloaded and cannot be saved.", cpos, chunk->chunkPos);
          continue;
        }
        
        // this is just in case, should not happen, I think.
        auto slices = generator.sliceMap.pop_if(cpos, [cpos = chunk->chunkPos](Structure::Slice const& slice) {
          return slice.origCpos == cpos;
        });
        uint32_t chunkPrio = generator.chunkPriority.sample1D(cpos);
        for(auto const& slice : slices) {
          bool override = generator.chunkPriority.sample1D(slice.origCpos) > chunkPrio;
          Structure::applySlice(*sliceChunk, slice, override);
        }

        spdlog::debug("save slice {}", cpos);
        SaveManager::saveSlice(*sliceChunk);
      }

      // now, we can forget in which chunks the slices have been generated.
      chunk->slices = {};
    }
    else {
      spdlog::warn("Cannot save unloaded chunk: {}", cpos);
    }
  }

  packet_blocks(client.uid, blocks);
}

void Server::updateWaitingChunks() {
  generator.waitingChunks.clear();
  size_t i = 0, j = 0;
  size_t count = 0;

  for(auto const& pair : clients) {
    count += pair.second.waitingChunks.size();
  }
  count = std::min(count, generator.waitingChunks.capacity());

  while(i < count) {
    for(auto const& pair : clients) {
      auto const& waiting = pair.second.waitingChunks;
      if(j < waiting.size()) {
        generator.waitingChunks.push(waiting.at(j));
        i++;
      }
    }
    j++;
  }
}

void Server::remOldChunks() {
  std::vector<ivec3> playersCpos;
  for(auto const& pair : clients) {
    ivec3 cpos = floor(pair.second.player.getPosition() / float(generator.chunkSize));
    playersCpos.push_back(cpos);
  }

  int delCount = std::max<int>(world.chunks.size() - maxChunks, 0);
  world.chunks.eraseChunks(delCount, [&](Chunk* chunk) {
    for(auto const& cpos : playersCpos) {
      ivec3 dist = abs(cpos - chunk->chunkPos);
      bool cond = true;
      cond &= dist.x <= renderDistH + 1;
      cond &= dist.z <= renderDistH + 1;
      cond &= dist.y <= renderDistV + 1;
      if(cond) return false;
    }
    generator.sliceMap.pop(chunk->chunkPos);
    return true;
  });
}

void Server::handleTimeouts() {
  std::vector<Identifier> erased;
  auto curTime = clock.getElapsedTime();

  for(auto it = clients.cbegin(); it != clients.cend(); ) {
    if(curTime - it->second.lastUpdate > timeout) {
      Identifier uid = it->second.uid;
      spdlog::warn("Client timeout: {}", uid);
      it = clients.erase(it);
      erased.push_back(uid);
      beep();
    }
    else ++it;
  }

  for(auto uid : erased)
    packet_logout(uid);
}
