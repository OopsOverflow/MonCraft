#include "Server.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include "../common/Packet.hpp"
#include <iostream>

Server::Server(std::string url, unsigned short port)
  : addr(url), port(port),
    frameDuration(sf::milliseconds(NetworkConfig::SERVER_TICK))
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

void Server::ping() {
  packet_ping();
}

void Server::packet_ping() {
  sf::Packet packet;
  PacketHeader header(ClientPacketType::PING);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    throw NetworkError("failed to ping server");
  }
}

void Server::packet_login() {
  sf::Packet packet;
  PacketHeader header(ClientPacketType::LOGIN);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    throw NetworkError("login failed");
  }
}

void Server::packet_logout() {
  sf::Packet packet;
  PacketHeader header(ClientPacketType::LOGOUT);

  packet << header;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] logout failed" << std::endl;
  }
}

void Server::packet_entity_tick(glm::vec3 playerPos) {
  sf::Packet packet;
  PacketHeader header(ClientPacketType::ENTITY_TICK);
  PacketEntityTick body(playerPos);

  packet << header << body;

  auto send_res = socket.send(packet, addr, port);

  if(send_res != sf::Socket::Done) {
    std::cout << "[WARN] logout failed" << std::endl;
  }
}
