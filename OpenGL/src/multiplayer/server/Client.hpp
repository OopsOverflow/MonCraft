#pragma once

#include <glm/glm.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Time.hpp>
#include <deque>

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
  Client(Identifier uid, sf::Time creation);

  Identifier uid;
  Entity player;
  std::deque<glm::ivec3> waitingChunks;
  bool ack;
  sf::Time lastUpdate;
};
