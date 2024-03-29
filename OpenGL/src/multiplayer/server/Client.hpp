#pragma once

#include <glm/glm.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <mutex>

#include "util/Identifier.hpp"
#include "entity/Entity.hpp"

class ClientID {
public:
  ClientID(std::string remoteAddress);

  bool operator==(ClientID const& lhs) const;
  bool operator<(ClientID const& lhs) const;

  std::string getAddr() const;
  // unsigned short getPort() const;


private:
  std::string addr;
  // unsigned short port;
};

class Client {
public:
  Client(Identifier uid, sf::Time creation);

  Identifier uid;
  Entity player;
  std::vector<glm::ivec3> waitingChunks;
  bool ack;
  sf::Time lastUpdate;
};
