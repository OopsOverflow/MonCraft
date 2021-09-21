#include "BlockArray.hpp"


sf::Packet& operator<<(sf::Packet& packet, BlockArrayElement const& blockData) {
  packet << blockData.pos;
  packet << (sf::Uint64)blockData.type;
  return packet;
}

sf::Packet& operator>>(sf::Packet& packet, BlockArrayElement& blockData) {
  sf::Uint64 type;
  packet >> blockData.pos;
  packet >> type;
  blockData.type = (BlockType)type;
  return packet;
}
