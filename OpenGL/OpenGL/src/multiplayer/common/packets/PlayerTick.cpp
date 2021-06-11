#include "../Packet.hpp"

PacketPlayerTick::PacketPlayerTick(glm::vec3 playerPos)
  : playerPos(playerPos)
{ }

PacketPlayerTick::PacketPlayerTick() : PacketPlayerTick(glm::vec3(0)) {}

glm::vec3 PacketPlayerTick::getPlayerPos() const {
  return playerPos;
}

sf::Packet& operator<<(sf::Packet& packet, PacketPlayerTick const& body) {
  return packet << body.playerPos;
}

sf::Packet& operator>>(sf::Packet& packet, PacketPlayerTick& body) {
  return packet >> body.playerPos;
}
