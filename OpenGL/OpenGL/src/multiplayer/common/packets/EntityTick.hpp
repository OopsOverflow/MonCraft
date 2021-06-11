#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include "entity/Node.hpp"
#include "../Identifier.hpp"

struct EntityData {
  glm::vec3 pos;
  Identifier uid;

  EntityData(glm::vec3 pos, Identifier uid);
  EntityData();
};

class PacketEntityTick {
public:
  PacketEntityTick();
  PacketEntityTick(std::vector<EntityData> entities);

  std::vector<EntityData> getPlayers() const;

private:
  std::vector<EntityData> entities;
  friend sf::Packet& operator<<(sf::Packet& packet, PacketEntityTick const& body);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketEntityTick& body);
};

sf::Packet& operator<<(sf::Packet& packet, EntityData const& entity);
sf::Packet& operator>>(sf::Packet& packet, EntityData& entity);
