#include <SFML/Network.hpp>
#include <glm/glm.hpp>

class PacketEntityTick {
public:
  PacketEntityTick();
  PacketEntityTick(glm::vec3 playerPos);

  glm::vec3 getPlayerPos() const;

private:
  glm::vec3 playerPos;
  friend sf::Packet& operator<<(sf::Packet& packet, PacketEntityTick const& body);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketEntityTick& body);
};
