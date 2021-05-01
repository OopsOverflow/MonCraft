#include "Cactus_Block.h"

Cactus_Block* Cactus_Block::get() {
  static Cactus_Block inst;
  return &inst;
}

Cactus_Block::Cactus_Block() : Block(BlockType::Cactus)
{ }

glm::ivec2 Cactus_Block::getFaceUVs(BlockFace face) const {
  if(face == BlockFace::TOP || face == BlockFace::BOTTOM)
    return {3, 0};
  return {4, 0};
}
