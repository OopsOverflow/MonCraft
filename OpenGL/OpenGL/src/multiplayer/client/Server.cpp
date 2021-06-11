#include "Server.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include "../common/Packet.hpp"
#include <iostream>

Server::Server(std::string url, unsigned short port)
  : addr(url), port(port),
    frameDuration(sf::milliseconds(NetworkConfig::SERVER_TICK)),
    uid(0)
{
  lastUpdate = clock.getElapsedTime() - frameDuration - frameDuration; // needs update
  packet_login();
}

Server::~Server() {
  packet_logout();
}


void Server::update(glm::vec3 playerPos) {
  sf::Time now = clock.getElapsedTime();
  if(now - lastUpdate > frameDuration) {
    packet_entity_tick(playerPos);
    lastUpdate = now;
  }
}

Identifier Server::getPlayerID() const {
  return uid;
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

void Server::packet_entity_tick(glm::vec3 playerPos) {
  sf::Packet packet;
  PacketHeader header(PacketType::ENTITY_TICK);
  PacketEntityTick body(playerPos);

  packet << header << body;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] entity_tick failed" << std::endl;
  }
}

void Server::listen_ack_login() {
  sf::Packet packet;
  sf::IpAddress serverAddr;
  unsigned short serverPort;

  auto recv_res = socket.receive(packet, serverAddr, serverPort);

  if(recv_res != sf::Socket::Done) {
    throw NetworkError("listen_ack_login failed");
  }

  PacketHeader header;
  packet >> header;
  PacketType type = header.getType();

  if(type != PacketType::ACK_LOGIN) {
    throw NetworkError("listen_ack_login failed");
  }

  PacketAckLogin body;
  packet >> body;
  uid = body.getIdentifier();
}
