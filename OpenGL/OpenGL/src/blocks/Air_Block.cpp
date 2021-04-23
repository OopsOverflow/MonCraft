#include "Air_Block.h"

Air_Block* Air_Block::get() {
  static Air_Block inst;
  return &inst;
}

Air_Block::Air_Block() : Block(BlockType::Air)
{ }

glm::ivec2 Air_Block::getFaceUVs(BlockFace face) const {
  return {0, 0};
}
