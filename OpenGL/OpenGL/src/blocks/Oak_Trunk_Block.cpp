#include "Oak_Trunk_Block.h"

Oak_Trunk_Block* Oak_Trunk_Block::get() {
  static Oak_Trunk_Block inst;
  return &inst;
}

Oak_Trunk_Block::Oak_Trunk_Block() : Block(BlockType::Oak_Trunk)
{ }

glm::ivec2 Oak_Trunk_Block::getFaceUVs(BlockFace face) const {
  if(face == BlockFace::TOP || face == BlockFace::BOTTOM)
    return {2, 1};
  return {3, 1};
}
