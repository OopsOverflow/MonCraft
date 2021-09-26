#include "RealServer.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include <iostream>
#include "debug/Debug.hpp"
#include "blocks/AllBlocks.hpp"
#include "save/SaveManager.hpp"

using namespace glm;

RealServer::RealServer(std::string url, unsigned short port)
  : addr(url), port(port),
    frameDuration(sf::milliseconds(NetworkConfig::SERVER_TICK)),
    world(World::getInst())
{
  lastUpdate = clock.getElapsedTime() - frameDuration - frameDuration; // nqeeds update
  socket.setBlocking(false);
  if(socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
    throw NetworkError("Failed to bind to any port");
  }

  bool handshake = false;
  do {
    packet_login();
    std::cout << "waiting for server..." << std::endl;
    handshake = listen_ack_login();
    sf::sleep(sf::milliseconds(100));
  } while(!handshake);

  auto newPlayer = std::make_unique<Character>(NetworkConfig::SPAWN_POINT);
  auto entity = world.entities.add(playerUid, std::move(newPlayer));
  player = std::dynamic_pointer_cast<Character>(entity);
}

RealServer::~RealServer() {
  packet_logout();
}

void RealServer::update() {
  while(poll()) {};

  packet_blocks();
  packet_chunks();

  pendingChunks.remOldChunks();

  sf::Time now = clock.getElapsedTime();
  if(now - lastUpdate > frameDuration) {
    packet_player_tick();
    lastUpdate = now;
  }
}

bool RealServer::poll() {
  sf::Packet packet;
  sf::IpAddress serverAddr;
  unsigned short serverPort;

  auto recv_res = socket.receive(packet, serverAddr, serverPort);

  if(recv_res != sf::Socket::Done) {
    return false;
  }

  PacketHeader header;
  packet >> header;
  auto type = header.getType();

  if(type == PacketType::ENTITY_TICK) applyEntityTransforms(packet);
  else if(type == PacketType::LOGOUT) handle_logout(packet);
  else if(type == PacketType::BLOCKS) handle_blocks(packet);
  else if(type == PacketType::CHUNKS) handle_chunks(packet);
  else {
    std::cout << "[WARN] unhandled packed: " << header << std::endl;
  }

  return true;
}

void RealServer::applyEntityTransforms(sf::Packet& packet) {
  sf::Uint64 size;
  packet >> size;

  for(sf::Uint64 i = 0; i < size; i++) {
    Identifier uid;
    packet >> uid;

    auto entity = world.entities.get(uid);

    if(uid == playerUid) {
      entity->consume(packet);
    }
    else {
      if(entity == nullptr) { // create the player if not found
        world.entities.add(uid, std::make_unique<Character>(NetworkConfig::SPAWN_POINT));
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
  auto blocks = player->getRecord();
  if(blocks.size() != 0) {
    sf::Packet packet;
    PacketHeader header(PacketType::BLOCKS);
    packet << header << blocks;

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

  packet << header;

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
  if(!pendingChunks.changed(player->getPosition())) return;
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

bool RealServer::listen_ack_login() {
  sf::Packet packet;
  sf::IpAddress serverAddr;
  unsigned short serverPort;
  auto recv_res = socket.receive(packet, serverAddr, serverPort);

  if(recv_res != sf::Socket::Done) {
    return false;
  }

  PacketHeader header;
  packet >> header;
  PacketType type = header.getType();

  if(type != PacketType::ACK_LOGIN) {
    throw NetworkError("listen_ack_login failed");
  }

  packet >> playerUid;

  return true;
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

    for(auto const& blockData : blocks) {
      world.setBlock(blockData.pos, AllBlocks::create_static(blockData.type));
    }
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
    auto newChunk = std::make_unique<Chunk>(chunkPos, chunkSize);
    packet >> *newChunk;

    if(!world.chunks.find(chunkPos)) {
      auto chunk = world.chunks.insert(chunkPos, std::move(newChunk));

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
