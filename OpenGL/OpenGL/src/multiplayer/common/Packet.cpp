#include "Packet.hpp"

// header

PacketHeader::PacketHeader(ClientPacketType type)
  : type((sf::Uint8)type)
{ }

PacketHeader::PacketHeader()
  : PacketHeader(ClientPacketType::NONE)
{ }


ClientPacketType PacketHeader::getType() const {
  return (ClientPacketType)type;
}

sf::Packet& operator<<(sf::Packet& packet, PacketHeader const& header) {
    return packet << header.type;
}

sf::Packet& operator>>(sf::Packet& packet, PacketHeader& header) {
    return packet >> header.type;
}

std::ostream& operator<<(std::ostream& os, PacketHeader const& header) {
  ClientPacketType type = header.getType();

  if(type == ClientPacketType::LOGIN) os << "LOGIN";
  else if(type == ClientPacketType::LOGOUT) os << "LOGOUT";
  else if(type == ClientPacketType::ENTITY_TICK) os << "ENTITY_TICK";
  else if(type == ClientPacketType::PING) os << "PING";
  else if(type == ClientPacketType::NONE) os << "NONE";

  return os;
}

// entity tick

PacketEntityTick::PacketEntityTick(glm::vec3 playerPos)
  : playerPos(playerPos)
{ }

PacketEntityTick::PacketEntityTick() : PacketEntityTick(glm::vec3(0)) {}

glm::vec3 PacketEntityTick::getPlayerPos() const {
  return playerPos;
}

sf::Packet& operator<<(sf::Packet& packet, PacketEntityTick const& body) {
  return packet << body.playerPos;
}

sf::Packet& operator>>(sf::Packet& packet, PacketEntityTick& body) {
  return packet >> body.playerPos;
}
