#include "Dirt_Block.h"

Dirt_Block::Dirt_Block()
    : Block(BlockType::Dirt)
{}

glm::ivec2 Dirt_Block::getFaceUVs(BlockFace face) const {
  return {1, 0};
}
