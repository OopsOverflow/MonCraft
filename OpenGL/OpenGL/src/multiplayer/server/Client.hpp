#pragma once

#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include "util/Identifier.hpp"
#include "entity/Entity.hpp"

class ClientID {
public:
  ClientID(sf::IpAddress addr, unsigned short port);

  bool operator==(ClientID const& lhs) const;
  bool operator<(ClientID const& lhs) const;

  sf::IpAddress getAddr() const;
  unsigned short getPort() const;

private:
  sf::IpAddress addr;
  unsigned short port;
};

class Client {
public:
  Client(Identifier uid);

  Entity player;
  std::vector<glm::ivec3> waitingChunks;
};
