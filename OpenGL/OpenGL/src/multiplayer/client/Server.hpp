#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include "../common/Identifier.hpp"

class Server {

public:
  Server(std::string url, unsigned short port);
  ~Server();

  void ping();
  void update(glm::vec3 playerPos);
  Identifier getPlayerID() const;

private:
  void packet_login();
  void packet_logout();
  void packet_ping();
  void packet_entity_tick(glm::vec3 playerPos);
  void listen_ack_login();

  sf::IpAddress addr;
  unsigned short port;
  sf::UdpSocket socket;
  sf::Time lastUpdate;
  const sf::Time frameDuration;
  sf::Clock clock;
  Identifier uid;
};
