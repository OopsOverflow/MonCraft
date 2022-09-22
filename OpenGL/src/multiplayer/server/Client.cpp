#include "Client.hpp"
#include "save/ServerConfig.hpp"
#include "entity/character/CharacterHitbox.hpp"


ClientID::ClientID(std::string remoteAddress)
  : addr(remoteAddress)
{ }


bool ClientID::operator==(ClientID const& lhs) const {
  return addr == lhs.addr;
}

bool ClientID::operator<(ClientID const& lhs) const {
  return addr < lhs.addr;
}


Client::Client(Identifier uid, sf::Time creation)
  : uid(uid), player(CharacterHitbox()), lastUpdate(creation)
{
  player.setPosition(Config::getServerConfig().spawnPoint);
}


std::string ClientID::getAddr() const {
  // static const sf::IpAddress localAddr("192.168.0.254");
  // if(addr == localAddr) return sf::IpAddress::LocalHost;
  return addr;
}

// unsigned short ClientID::getPort() const {
//   return port;
// }
