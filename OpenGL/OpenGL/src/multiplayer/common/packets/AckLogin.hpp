#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include "../../common/Identifier.hpp"

class PacketAckLogin {
public:
  PacketAckLogin();
  PacketAckLogin(Identifier uid);

  Identifier getIdentifier() const;

private:
  Identifier uid;

  friend sf::Packet& operator<<(sf::Packet& packet, PacketAckLogin const& body);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketAckLogin& body);
};
