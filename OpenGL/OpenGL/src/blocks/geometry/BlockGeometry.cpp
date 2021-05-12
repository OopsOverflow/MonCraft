#include <algorithm>

#include "BlockGeometry.hpp"
#include "blocks/Block.hpp"

BlockGeometry::BlockGeometry() {}
BlockGeometry::~BlockGeometry() {}

face_t<2> BlockGeometry::genFaceUV(glm::ivec2 index) const {
  static const int atlasSize = 8;

  return face_t<2>{
    (index.x + 1.f) / atlasSize, (index.y + 0.f) / atlasSize,
    (index.x + 0.f) / atlasSize, (index.y + 0.f) / atlasSize,
    (index.x + 0.f) / atlasSize, (index.y + 1.f) / atlasSize,
    (index.x + 1.f) / atlasSize, (index.y + 1.f) / atlasSize,
  };
}
