#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "blocks/Block.hpp"
#include "multiplayer/common/Packet.hpp"

struct BlockArrayElement {
  glm::ivec3 pos;
  BlockType type;

  friend sf::Packet& operator<<(sf::Packet& packet, BlockArrayElement const& blockData);
  friend sf::Packet& operator>>(sf::Packet& packet, BlockArrayElement& blockData);
};

using BlockArray = std::vector<BlockArrayElement>;
