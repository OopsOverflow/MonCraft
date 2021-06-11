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
    packet_player_tick(entities->player.character->getPosition());
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
    PacketEntityTick body;
    packet >> body;

    for(auto const& serverPlayer : body.getPlayers()) {
      if(serverPlayer.uid == entities->player.uid) {
        // std::cout << "server pos: " << serverPlayer.pos << std::endl;
      }
      else {
        auto const& players = entities->players;

        auto it = std::find_if(players.begin(), players.end(), [&] (Player const& p) {
          return p.uid == serverPlayer.uid;
        });

        if(it != players.end()) {
          it->character->setPosition(serverPlayer.pos);
        }
        else {
          entities->players.emplace_back(serverPlayer.uid, serverPlayer.pos);
        }
      }
    }
  }

  return true;
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

void Server::packet_player_tick(glm::vec3 playerPos) {
  sf::Packet packet;
  PacketHeader header(PacketType::PLAYER_TICK);
  PacketPlayerTick body(playerPos);

  packet << header << body;

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

  PacketAckLogin body;
  packet >> body;
  entities->player.uid = body.getIdentifier();

  return true;
}
