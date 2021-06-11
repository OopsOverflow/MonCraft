#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include <functional>
#include "../common/Identifier.hpp"
#include "../common/Packet.hpp"
#include "entity/character/Character.hpp"
#include "SharedEntities.hpp"

class Server {

public:
  Server(std::string url, unsigned short port);
  ~Server();

  void ping();
  void update();
  entities_ptr_t getEntities() const;


private:
  void packet_login();
  void packet_logout();
  void packet_ping();
  void packet_player_tick(glm::vec3 playerPos);
  bool listen_ack_login();
  bool poll();

  sf::IpAddress addr;
  unsigned short port;
  sf::UdpSocket socket;
  sf::Time lastUpdate;
  const sf::Time frameDuration;
  sf::Clock clock;

  entities_ptr_t entities;
};
