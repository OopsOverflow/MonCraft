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
#include "terrain/ChunkImpl.hpp"
#include "terrain/BlockArray.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"

using namespace glm;
using namespace serde;
using std::placeholders::_1;

static const rtc::Configuration config {

};

static const rtc::DataChannelInit init {
  .reliability = {
    .type = rtc::Reliability::Type::Rexmit,
    .unordered = true,
    .rexmit = 0
  }
};

RealServer::RealServer(std::string addr, unsigned short port, bool tls)
  : addr(addr), port(port),
    peer(nullptr), channel(nullptr),
    frameDuration(sf::milliseconds(Config::getServerConfig().serverTick)),
    world(World::getInst()),
    playerUid(generateIdentifier()),
    serverAck(false)
{
  lastUpdate = clock.getElapsedTime() - frameDuration - frameDuration; // needs update
  lastServerUpdate = clock.getElapsedTime();

  auto newPlayer = std::make_unique<Character>(Config::getServerConfig().spawnPoint);
  auto entity = world.entities.add(playerUid, std::move(newPlayer));
  player = std::static_pointer_cast<Character>(entity);

  // ---- websocket setup ----

  socket.onMessage(std::bind(&RealServer::on_message, this, _1));
  socket.onError([] (std::string e) { std::cerr << "[ERR] WebSocket error! " << e << std::endl; });
  socket.onClosed([] () { std::cout << "[INFO] WebSocket closed" << std::endl; });

  socket.onOpen([this] () {
    std::cout << "[INFO] WebSocket open" << std::endl;

    peer = std::make_unique<rtc::PeerConnection>(config);
  	peer->onLocalDescription([this](rtc::Description description) { socket.send(description); });
  	peer->onLocalCandidate([this](rtc::Candidate candidate) {socket.send(candidate); });
  	// peer->onLocalDescription([this](rtc::Description description) { std::cout << "[INFO] Peer Local Description" << std::endl; socket.send(description); });
  	// peer->onLocalCandidate([this](rtc::Candidate candidate) { std::cout << "[INFO] Peer Local Candidate: " << candidate << std::endl; socket.send(candidate); });
  	// peer->onStateChange([](rtc::PeerConnection::State state) { std::cout << "[INFO] Peer State: " << state << std::endl; });
  	// peer->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) { std::cout << "[INFO] Peer Gathering State: " << state << std::endl; });

    channel = peer->createDataChannel(std::to_string(getUid()));
    channel->onMessage(std::bind(&RealServer::on_message, this, _1));
    channel->onError([] (std::string e) { std::cerr << "[ERR] DataChannel error! " << e << std::endl; });
    channel->onClosed([] () { std::cout << "[INFO] DataChannel closed" << std::endl; });
    channel->onOpen([] () { std::cout << "[INFO] DataChannel open" << std::endl; });

    packet_login();
  });

  std::string url = "ws" + std::string(tls ? "s" : "") + "://" + addr + ":" + std::to_string(port);
  std::cout << "[INFO] connecting to websocket server at " << url << std::endl;
  state = ServerState::CONNECTING;
  socket.open(url);
}

RealServer::~RealServer() {
  packet_logout();
}

bool RealServer::send(sf::Packet& packet) {
  if(channel && channel->isOpen()) // prefer webrtc over websocket
    return channel->send((std::byte*)packet.getData(), packet.getDataSize());
  else if(socket.isOpen())
    return socket.send((std::byte*)packet.getData(), packet.getDataSize());
  else
    return false;
}

void RealServer::on_message(rtc::message_variant msg) {
  if (std::holds_alternative<std::string>(msg)) {
    auto data = std::get<std::string>(msg);
    if(data.starts_with("a=candidate")) {
      rtc::Candidate candidate(data, "MonCraft");
      // std::cout << "[INFO] Peer Remote candidate: " << candidate << std::endl;
      peer->addRemoteCandidate(candidate);
    }
    else {
      rtc::Description description(data, rtc::Description::Type::Answer);
      // std::cout << "[INFO] Peer Remote description" << std::endl;
      peer->setRemoteDescription(description);
    }
  }
  else {
    auto data = std::get<rtc::binary>(msg);
    sf::Packet packet;
    packet.append(data.data(), data.size());
    on_packet_recv(packet);
  }
}

void RealServer::update() {
  Server::update();

  if(clock.getElapsedTime() - lastServerUpdate > timeout) {
    state = ServerState::DISCONNECTED;
    return;
  }

  packet_blocks();
  packet_chunks();

  sf::Time now = clock.getElapsedTime();
  if(now - lastUpdate > frameDuration && serverAck) {
    packet_player_tick();
    lastUpdate = now;
    serverAck = false;
  }
}

bool RealServer::on_packet_recv(sf::Packet& packet) {
  PacketHeader header;
  packet >> header;
  auto type = header.getType();
  lastServerUpdate = clock.getElapsedTime();
  serverAck = true;

  if(type == PacketType::ACK_LOGIN) {
    state = ServerState::CONNECTED;
    std::cout << "[INFO] logged into the server" << std::endl;
  } 
  else if(type == PacketType::ENTITY_TICK) handle_entity_tick(packet);
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

    auto send_res = send(packet);

    if(!send_res) {
      throw NetworkError("failed to send blocks to server");
    }
  }
}

void RealServer::packet_ping() {
  sf::Packet packet;
  PacketHeader header(PacketType::PING);

  packet << header;

  auto send_res = send(packet);

  if(!send_res) {
    throw NetworkError("failed to ping server");
  }
}

void RealServer::packet_login() {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGIN);

  packet << header << playerUid;

  auto send_res = send(packet);

  if(!send_res) {
    throw NetworkError("login failed");
  }
}

void RealServer::packet_logout() {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGOUT);

  packet << header;

  auto send_res = send(packet);

  if(!send_res) {
    std::cout << "[WARN] logout failed" << std::endl;
  }
}

void RealServer::packet_player_tick() {
  sf::Packet packet;
  PacketHeader header(PacketType::PLAYER_TICK);
  packet << header << *player;

  auto send_res = send(packet);

  if(!send_res) {
    std::cout << "[WARN] player_tick failed" << std::endl;
  }
}

void RealServer::packet_chunks() {
  pendingChunks.update(player->getPosition());
  if(!pendingChunks.changed()) return;
  sf::Packet packet;
  PacketHeader header(PacketType::CHUNKS);
  packet << header << pendingChunks.get();

  auto send_res = send(packet);

  if(!send_res) {
    std::cout << "[WARN] chunks failed" << std::endl;
  }
}

void RealServer::packet_ack_chunks(std::vector<glm::ivec3> ack) {
  if(ack.size() == 0) return;

  sf::Packet packet;
  PacketHeader header(PacketType::ACK_CHUNKS);
  packet << header << ack;

  auto send_res = send(packet);

  if(!send_res) {
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
    auto newChunk = new ChunkImpl(chunkPos, chunkSize);
    packet >> *newChunk;

    if(!world.chunks.find(chunkPos)) {
      auto chunk = world.chunks.insert(chunkPos, std::unique_ptr<ChunkImpl>(newChunk));

      for(size_t j = 0; j < 26; j++) {
        if(!chunk->neighbors[j].lock()) {
          ivec3 thisPos = chunkPos + Chunk::neighborOffsets[j];
          if(auto neigh = world.chunks.find(thisPos)) {
            chunk->neighbors[j] = neigh;
            neigh->neighbors[Chunk::neighborOffsetsInverse[j]] = chunk;
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
