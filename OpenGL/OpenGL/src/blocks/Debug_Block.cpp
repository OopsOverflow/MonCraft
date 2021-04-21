#include "Debug_Block.h"

Debug_Block::Debug_Block() : Block(BlockType::Grass)
{ }

glm::ivec2 Debug_Block::getFaceUVs(BlockFace face) const {
  if(face == BlockFace::TOP)
    return {0, 7};
  else if(face == BlockFace::BOTTOM)
    return {1, 7};
  else if(face == BlockFace::FRONT)
    return {2, 7};
  else if(face == BlockFace::RIGHT)
    return {3, 7};
  else if(face == BlockFace::BACK)
    return {4, 7};
  else
    return {5, 7};
}
