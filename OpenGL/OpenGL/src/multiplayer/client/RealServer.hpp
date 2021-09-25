#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include <functional>
#include "util/Identifier.hpp"
#include "../common/Packet.hpp"
#include "entity/character/Character.hpp"
#include "Server.hpp"

class RealServer: public Server {

public:
  RealServer(std::string url, unsigned short port);
  virtual ~RealServer();

  void ping() override;
  void update() override;
  std::shared_ptr<Character> getPlayer() override;

private:
  void packet_login();
  void packet_logout();
  void packet_ping();
  void packet_player_tick();
  void packet_blocks();
  bool listen_ack_login();
  void handle_logout(sf::Packet& packet);
  void handle_blocks(sf::Packet& packet);
  bool poll();
  void applyEntityTransforms(sf::Packet& packet);

  sf::IpAddress addr;
  unsigned short port;
  sf::UdpSocket socket;
  sf::Time lastUpdate;
  const sf::Time frameDuration;
  sf::Clock clock;
  World& world;
  std::shared_ptr<Character> player;
  Identifier playerUid;
};
