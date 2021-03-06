#include "Client.hpp"
#include "../common/Config.hpp"
#include "entity/character/CharacterHitbox.hpp"


ClientID::ClientID(sf::IpAddress addr, unsigned short port)
  : addr(addr), port(port)
{ }


bool ClientID::operator==(ClientID const& lhs) const {
  return port == lhs.port && addr == lhs.addr;
}

bool ClientID::operator<(ClientID const& lhs) const {
  if(port == lhs.port) {
    return addr < lhs.addr;
  }
  return port < lhs.port;
}


Client::Client(Identifier uid)
  : player(uid, NetworkConfig::SPAWN_POINT, CharacterHitbox())
{ }


sf::IpAddress ClientID::getAddr() const {
  static const sf::IpAddress localAddr("192.168.0.254");
  if(addr == localAddr) return sf::IpAddress::LocalHost;
  return addr;
}

unsigned short ClientID::getPort() const {
  return port;
}
