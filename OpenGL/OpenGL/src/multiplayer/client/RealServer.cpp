#include "RealServer.hpp"

#include <SFML/Config.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <glm/glm.hpp>
#include <stddef.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "entity/Entities.hpp"
#include "entity/Entity.hpp"
#include "entity/character/Character.hpp"
#include "multiplayer/NetworkError.hpp"
#include "multiplayer/Packet.hpp"
#include "multiplayer/Serialize.hpp"
#include "save/ServerConfig.hpp"
#include "terrain/AbstractChunk.hpp"
#include "terrain/BlockArray.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"

using namespace glm;
using namespace serde;

RealServer::RealServer(std::string url, unsigned short port)
  : addr(url), port(port),
    frameDuration(sf::milliseconds(Config::getServerConfig().serverTick)),
    world(World::getInst()),
    playerUid(generateIdentifier()),
    serverAck(true)
{
  lastUpdate = clock.getElapsedTime() - frameDuration - frameDuration; // needs update

  socket.setBlocking(false);

  if(socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
    throw NetworkError("Failed to bind to any port");
  }

  lastServerUpdate = clock.getElapsedTime();

  auto newPlayer = std::make_unique<Character>(Config::getServerConfig().spawnPoint);
  auto entity = world.entities.add(playerUid, std::move(newPlayer));
  player = std::dynamic_pointer_cast<Character>(entity);
}

RealServer::~RealServer() {
  packet_logout();
}

bool RealServer::login() {
  sf::Packet packet;
  sf::IpAddress serverAddr;
  unsigned short serverPort;

  auto recv_res = socket.receive(packet, serverAddr, serverPort);

  if(recv_res == sf::Socket::Done) {
    PacketHeader header;
    packet >> header;
    auto type = header.getType();
    lastServerUpdate = clock.getElapsedTime();

    if(type == PacketType::ACK_LOGIN) return true; // TODO: check correct uid ?
    else std::cout << "[WARN] not a login packet: " << header << std::endl;
  }

  packet_login();
  return false;
}

void RealServer::update() {
  Server::update();
  
  if(clock.getElapsedTime() - lastServerUpdate > timeout) {
    throw std::runtime_error("server timeout");
  }

  #ifdef EMSCRIPTEN
    poll(); // TODO: this hack avoids waiting too long and timeout
  #else
    while(poll()) {}
  #endif

  packet_blocks();
  packet_chunks();

  sf::Time now = clock.getElapsedTime();
  if(now - lastUpdate > frameDuration && serverAck) {
    packet_player_tick();
    lastUpdate = now;
    serverAck = false;
  }
}

bool RealServer::poll() {
  sf::Packet packet;
  sf::IpAddress serverAddr;
  unsigned short serverPort;

  auto recv_res = socket.receive(packet, serverAddr, serverPort);

  if(recv_res != sf::Socket::Done) return false;

  PacketHeader header;
  packet >> header;
  auto type = header.getType();
  lastServerUpdate = clock.getElapsedTime();
  serverAck = true;

  if(type == PacketType::ENTITY_TICK) handle_entity_tick(packet);
  else if(type == PacketType::LOGOUT) handle_logout(packet);
  else if(type == PacketType::BLOCKS) handle_blocks(packet);
  else if(type == PacketType::CHUNKS) handle_chunks(packet);
  else std::cout << "[WARN] unhandled packet: " << header << std::endl;

  return true;
}

void RealServer::handle_entity_tick(sf::Packet& packet) {
  sf::Uint64 size;
  packet >> size;

  for(sf::Uint64 i = 0; i < size; i++) {
    Identifier uid;
    packet >> uid;

    auto entity = world.entities.get(uid);

    if(uid == playerUid) {
      consume(*entity, packet);
    }
    else {
      if(entity == nullptr) { // create the player if not found
        world.entities.add(uid, std::make_unique<Character>(Config::getServerConfig().spawnPoint));
        entity = world.entities.get(uid);
      }

      packet >> *entity;
    }
  }
}

void RealServer::ping() {
  packet_ping();
}

void RealServer::packet_blocks() {
  BlockArray& blocks = player->getRecord();
  if(!blocks.empty()) {
    sf::Packet packet;
    PacketHeader header(PacketType::BLOCKS);
    packet << header << blocks;
    blocks.clear();

    auto send_res = socket.send(packet, addr, port);

    if(send_res != sf::Socket::Done) {
      throw NetworkError("failed to send blocks to server");
    }
  }
}

void RealServer::packet_ping() {
  sf::Packet packet;
  PacketHeader header(PacketType::PING);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    throw NetworkError("failed to ping server");
  }
}

void RealServer::packet_login() {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGIN);

  packet << header << playerUid;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    throw NetworkError("login failed");
  }
}

void RealServer::packet_logout() {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGOUT);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] logout failed" << std::endl;
  }
}

void RealServer::packet_player_tick() {
  sf::Packet packet;
  PacketHeader header(PacketType::PLAYER_TICK);
  packet << header << *player;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] player_tick failed" << std::endl;
  }
}

void RealServer::packet_chunks() {
  pendingChunks.update(player->getPosition());
  if(!pendingChunks.changed()) return;
  sf::Packet packet;
  PacketHeader header(PacketType::CHUNKS);
  packet << header << pendingChunks.get();

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] chunks failed" << std::endl;
  }
}

void RealServer::packet_ack_chunks(std::vector<glm::ivec3> ack) {
  if(ack.size() == 0) return;

  sf::Packet packet;
  PacketHeader header(PacketType::ACK_CHUNKS);
  packet << header << ack;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] ack_chunks failed" << std::endl;
  }
}

void RealServer::handle_logout(sf::Packet& packet) {
  Identifier uid;
  packet >> uid;

  if(!world.entities.remove(uid)) {
    std::cout << "[WARN] logout of unknown player" << std::endl;
  }
}

void RealServer::handle_blocks(sf::Packet& packet) {
  Identifier uid;
  packet >> uid;

  if(uid != playerUid) {
    BlockArray blocks;
    packet >> blocks;
    blocks.copyToWorld();
  }
}

void RealServer::handle_chunks(sf::Packet& packet) {
  sf::Uint8 chunkCount;
  packet >> chunkCount;
  std::vector<glm::ivec3> ack;

  for (size_t i = 0; i < chunkCount; i++) {
    sf::Uint8 chunkSize;
    glm::ivec3 chunkPos;
    packet >> chunkSize >> chunkPos;
    auto newChunk = AbstractChunk::create(chunkPos, chunkSize);
    packet >> *newChunk;

    if(!world.chunks.find(chunkPos)) {
      auto chunk = world.chunks.insert(chunkPos, std::unique_ptr<AbstractChunk>(newChunk));

      for(size_t j = 0; j < 26; j++) {
        if(!chunk->neighbors[j].lock()) {
          ivec3 thisPos = chunkPos + AbstractChunk::neighborOffsets[j];
          if(auto neigh = world.chunks.find(thisPos)) {
            chunk->neighbors[j] = neigh;
            neigh->neighbors[AbstractChunk::neighborOffsetsInverse[j]] = chunk;
            if(neigh->hasAllNeighbors()) {
              neigh->compute();
            }
          }
        }
      }

      if(chunk->hasAllNeighbors()) {
        chunk->compute();
      }
    }

    ack.push_back(chunkPos);
  }

  packet_ack_chunks(ack);
}

std::shared_ptr<Character> RealServer::getPlayer() {
  return player;
}

Identifier RealServer::getUid() {
  return playerUid;
}
