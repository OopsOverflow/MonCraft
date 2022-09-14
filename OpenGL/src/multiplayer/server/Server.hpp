#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <map>
#include <mutex>

#include "multiplayer/terrain/TerrainGenerator.hpp"
#include "multiplayer/server/Client.hpp"
#include "util/Identifier.hpp"

class BlockArray;
class World;
namespace sf { class Packet; }

class Server {

public:
  Server(unsigned short port);
  virtual ~Server();

  virtual void run() = 0;

protected:
  virtual bool send(sf::Packet& packet, ClientID client) = 0;
  void on_packet_recv(sf::Packet& packet, ClientID client);
  void on_server_tick();

private:
  void handle_login(ClientID client, sf::Packet& packet);
  void handle_logout(ClientID client);
  void handle_ping(Client& client);
  void handle_player_tick(Client& client, sf::Packet& packet);
  void handle_blocks(Client const& client, sf::Packet& packet);
  void handle_chunks(Client& client, sf::Packet& packet);
  void handle_ack_chunks(Client& client, sf::Packet& packet);

  void broadcast(sf::Packet& packet);
  void packet_entity_tick();
  void packet_logout(Identifier uid);
  void packet_ack_login(ClientID client, Identifier uid);
  void packet_blocks(Identifier uid, BlockArray changedBlocks);
  void packet_chunks();

  void beep();
  void updateWaitingChunks();
  void remOldChunks();
  void handleTimeouts();

protected:
  unsigned short port;
  std::map<ClientID, Client> clients;
  World& world;
  TerrainGenerator generator;

  int renderDistH, renderDistV;
  static const unsigned int maxChunks = 50;

  sf::Clock clock;
  const sf::Time timeout = sf::seconds(10);
  const sf::Time tickAckLimit = sf::milliseconds(1000);
  std::mutex mutex;
};
