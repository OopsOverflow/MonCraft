#include "RealServer.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include <iostream>
#include "debug/Debug.hpp"

RealServer::RealServer(std::string url, unsigned short port)
  : addr(url), port(port),
    frameDuration(sf::milliseconds(NetworkConfig::SERVER_TICK))
{
  lastUpdate = clock.getElapsedTime() - frameDuration - frameDuration; // needs update
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
}

RealServer::~RealServer() {
  packet_logout();
}

void RealServer::update() {
  while(poll()) {};

  packet_blocks();

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
  else {
    std::cout << "[WARN] unhandled packed: " << header << std::endl;
  }

  return true;
}

void RealServer::applyEntityTransforms(sf::Packet& packet) {
  auto& players = entities->players;

  sf::Uint64 size;
  packet >> size;

  for(sf::Uint64 i = 0; i < size; i++) {
    Identifier uid;
    packet >> uid;

    if(uid == entities->uid) {
      entities->player->consume(packet);
    }
    else {
      auto it = players.find(uid);

      // create the player if not found
      if(it == players.end()) {
        entities->createPlayer(uid);
        it = players.find(uid);
      }

      packet >> *it->second;
    }
  }
}

void RealServer::ping() {
  packet_ping();
}

void RealServer::packet_blocks() {
  auto blocks = entities->player->getRecord();
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
  packet << header << *entities->player;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] player_tick failed" << std::endl;
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
  
  packet >> entities->uid;

  return true;
}

void RealServer::handle_logout(sf::Packet& packet) {
  Identifier uid;
  packet >> uid;

  auto& players = entities->players;

  auto it = players.find(uid);
  if(it != players.end()) {
    players.erase(it);
  }
  else {
    std::cout << "[WARN] logout of unknown player" << std::endl;
  }
}

void RealServer::handle_blocks(sf::Packet& packet) {
  Identifier uid;
  packet >> uid;

  std::cout << uid << std::endl;

  if(uid != entities->uid) {
    BlockArray blocks;
    packet >> blocks;

    std::cout << blocks.size() << std::endl;

    for(auto const& blockData : blocks) {
      entities->terrain->setBlock(blockData.pos, AllBlocks::create_static(blockData.type));
    }
  }
}
