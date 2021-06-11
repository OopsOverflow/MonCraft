#include "Server.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include <iostream>
#include "debug/Debug.hpp"

Server::Server(std::string url, unsigned short port)
  : addr(url), port(port),
    frameDuration(sf::milliseconds(NetworkConfig::SERVER_TICK)),
    entities(SharedEntities::create())
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

Server::~Server() {
  packet_logout();
}

entities_ptr_t Server::getEntities() const {
  return entities;
}

void Server::update() {
  while(poll()) {};

  sf::Time now = clock.getElapsedTime();
  if(now - lastUpdate > frameDuration) {
    packet_player_tick();
    lastUpdate = now;
  }
}

bool Server::poll() {
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

  if(type == PacketType::ENTITY_TICK) {
    applyEntityTransforms(packet);
  }
  else if(type == PacketType::LOGOUT) {
    handle_logout(packet);
  }

  return true;
}

void Server::applyEntityTransforms(sf::Packet& packet) {
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
      if(it != players.end()) {
        auto player = entities->createPlayer();
        packet >> *it->second;
      }
      else {
        auto player = entities->createPlayer();
        packet >> *player;
        players.emplace(uid, std::move(player));
      }
    }
  }
}

void Server::ping() {
  packet_ping();
}

void Server::packet_ping() {
  sf::Packet packet;
  PacketHeader header(PacketType::PING);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    throw NetworkError("failed to ping server");
  }
}

void Server::packet_login() {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGIN);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    throw NetworkError("login failed");
  }
}

void Server::packet_logout() {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGOUT);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] logout failed" << std::endl;
  }
}

void Server::packet_player_tick() {
  sf::Packet packet;
  PacketHeader header(PacketType::PLAYER_TICK);
  packet << header << *entities->player;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] player_tick failed" << std::endl;
  }
}

bool Server::listen_ack_login() {
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

void Server::handle_logout(sf::Packet& packet) {
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
