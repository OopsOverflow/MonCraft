#include "Identifier.hpp"


static std::random_device rd;
static std::mt19937_64 gen(rd());

Identifier::Identifier(sf::Uint64 id)
  : id(id)
{ }

Identifier Identifier::generate() {
  return Identifier(gen());
}

sf::Packet& operator<<(sf::Packet& packet, Identifier const& uid) {
  return packet << uid.id;
}

sf::Packet& operator>>(sf::Packet& packet, Identifier& uid) {
  return packet >> uid.id;
}
