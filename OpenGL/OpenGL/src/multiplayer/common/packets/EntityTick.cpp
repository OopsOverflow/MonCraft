#include "../Packet.hpp"

EntityData::EntityData(glm::vec3 pos, Identifier uid)
  : pos(pos), uid(uid)
{ }

EntityData::EntityData()
  : EntityData(glm::vec3(0), Identifier())
{ }

PacketEntityTick::PacketEntityTick(std::vector<EntityData> entities)
  : entities(entities)
{ }

std::vector<EntityData> PacketEntityTick::getPlayers() const {
  return entities;
}

PacketEntityTick::PacketEntityTick() : PacketEntityTick(std::vector<EntityData>{}) {}

sf::Packet& operator<<(sf::Packet& packet, PacketEntityTick const& body) {
  return packet << body.entities;
}

sf::Packet& operator>>(sf::Packet& packet, PacketEntityTick& body) {
  return packet >> body.entities;
}

sf::Packet& operator<<(sf::Packet& packet, EntityData const& entity) {
  return packet << entity.pos << entity.uid;
}

sf::Packet& operator>>(sf::Packet& packet, EntityData& entity) {
  return packet >> entity.pos >> entity.uid;
}
