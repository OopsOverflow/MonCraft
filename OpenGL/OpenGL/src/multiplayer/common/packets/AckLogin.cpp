#include "../Packet.hpp"

PacketAckLogin::PacketAckLogin(Identifier uid)
  : uid(uid)
{ }

PacketAckLogin::PacketAckLogin() : PacketAckLogin(0) {}

Identifier PacketAckLogin::getIdentifier() const {
  return uid;
}

sf::Packet& operator<<(sf::Packet& packet, PacketAckLogin const& body) {
  return packet << body.uid;
}

sf::Packet& operator>>(sf::Packet& packet, PacketAckLogin& body) {
  return packet >> body.uid;
}
