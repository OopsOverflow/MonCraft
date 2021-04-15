#include "Grass_Block.h"

Grass_Block::Grass_Block() : Block(BlockType::Grass)
{ }

glm::ivec2 Grass_Block::getFaceUVs(BlockFace face) const {
  if(face == BlockFace::TOP)
    return {0, 0};
  else if(face == BlockFace::BOTTOM)
    return {1, 0};
  return {2, 0};
}
