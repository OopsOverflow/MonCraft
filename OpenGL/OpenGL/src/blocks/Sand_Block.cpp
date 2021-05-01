#include "Sand_Block.h"

Sand_Block* Sand_Block::get() {
  static Sand_Block inst;
  return &inst;
}

Sand_Block::Sand_Block()
    : Block(BlockType::Sand)
{}

glm::ivec2 Sand_Block::getFaceUVs(BlockFace face) const {
  return {1, 1};
}
