#include "../Packet.hpp"

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
