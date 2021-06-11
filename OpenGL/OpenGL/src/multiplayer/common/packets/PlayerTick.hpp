#include <SFML/Network.hpp>
#include <glm/glm.hpp>

class PacketPlayerTick {
public:
  PacketPlayerTick();
  PacketPlayerTick(glm::vec3 playerPos);

  glm::vec3 getPlayerPos() const;

private:
  glm::vec3 playerPos;
  friend sf::Packet& operator<<(sf::Packet& packet, PacketPlayerTick const& body);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketPlayerTick& body);
};
