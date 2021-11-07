#include "Client.hpp"
#include "save/ServerConfig.hpp"
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


Client::Client(Identifier uid, sf::Time creation)
  : uid(uid), player(CharacterHitbox()), lastUpdate(creation)
{
  player.setPosition(Config::getServerConfig().spawnPoint);
}


sf::IpAddress ClientID::getAddr() const {
  static const sf::IpAddress localAddr("192.168.0.254");
  if(addr == localAddr) return sf::IpAddress::LocalHost;
  return addr;
}

unsigned short ClientID::getPort() const {
  return port;
}
