#include "Stone_Block.h"

Stone_Block::Stone_Block()
    : Block(BlockType::Stone)
{}

  glm::ivec2 Stone_Block::getFaceUVs(BlockFace face) const {
    return {0, 1};
  }
