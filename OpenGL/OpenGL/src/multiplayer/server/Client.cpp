#include "Client.hpp"
#include "../common/Config.hpp"


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


Client::Client()
  // : player(NetworkConfig::SPAWN_POINT)
{

}
