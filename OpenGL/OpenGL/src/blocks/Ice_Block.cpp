#include "Ice_Block.h"

Ice_Block* Ice_Block::get() {
  static Ice_Block inst;
  return &inst;
}

Ice_Block::Ice_Block()
    : Block(BlockType::Ice)
{}

glm::ivec2 Ice_Block::getFaceUVs(BlockFace face) const {
  return {0, 2};
}
