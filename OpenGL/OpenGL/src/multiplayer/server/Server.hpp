#pragma once

#include <string>
#include <map>
#include <SFML/Network.hpp>
#include "../common/Packet.hpp"
#include "terrain/BlockArray.hpp"
#include "terrain/World.hpp"
#include "multiplayer/terrain/TerrainGenerator.hpp"

#include "Client.hpp"

class Server {

public:
  Server(unsigned short port);

  void run();

private:
  void broadcast(sf::Packet& packet);
  bool poll();
  void handle_login(sf::IpAddress clientAddr, unsigned short clientPort);
  void handle_logout(sf::IpAddress clientAddr, unsigned short clientPort);
  void handle_ping(Client& client);
  void handle_player_tick(Client& client, sf::Packet& packet);
  void handle_blocks(Client const& client, sf::Packet& packet);
  void handle_chunks(Client& client, sf::Packet& packet);
  void handle_ack_chunks(Client& client, sf::Packet& packet);
  void packet_entity_tick();
  void packet_logout(Identifier uid);
  void packet_ack_login(ClientID const& client, Identifier uid);
  void packet_blocks(Identifier uid, BlockArray changedBlocks);
  void packet_chunks();

  void beep();
  void updateWaitingChunks();
  void remOldChunks();

  unsigned short port;
  sf::UdpSocket socket;
  std::map<ClientID, Client> clients;
  World& world;
  TerrainGenerator generator;

  int renderDistH, renderDistV;
  static const unsigned int maxChunks = 50;
};
