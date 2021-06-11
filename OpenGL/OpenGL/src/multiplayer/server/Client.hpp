#pragma once

#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include "entity/character/Character.hpp"

class ClientID {
public:
  ClientID(sf::IpAddress addr, unsigned short port);

  sf::IpAddress addr;
  unsigned short port;

  bool operator==(ClientID const& lhs) const;
  bool operator<(ClientID const& lhs) const;
};

class Client {
public:
  Client();

  // Character player;
};
