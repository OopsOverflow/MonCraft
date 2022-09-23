#include "Structure.hpp"

#include <glm/glm.hpp>

#include "blocks/AllBlocks.hpp"
#include "ChunkImpl.hpp"

using namespace glm;

void Structure::applySlice(Chunk& chunk, Slice const& slice, bool override) {
  ivec3 dpos;
  for(dpos.x = slice.start.x; dpos.x < slice.end.x; dpos.x++) {
    for(dpos.y = slice.start.y; dpos.y < slice.end.y; dpos.y++) {
      for(dpos.z = slice.start.z; dpos.z < slice.end.z; dpos.z++) {
        if(!override && chunk[dpos]->type != BlockType::Air) continue;
        auto blockType = (*slice.store)[slice.off + dpos - slice.start];
        if(blockType != BlockType::Air)
          chunk[dpos] = AllBlocks::create_static(blockType);
      }
    }
  }
}

