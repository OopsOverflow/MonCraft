#include "Structure.hpp"
#include "blocks/Dirt_Block.hpp"
#include "blocks/Leaf_Block.hpp"
#include "blocks/Stone_Block.hpp"
#include "blocks/AllBlocks.hpp"

using namespace glm;

Tree::Tree()
 : store(std::make_shared<BlockStore>(ivec3(5, 7, 5)))
{
  ivec3 pos;
  for(pos.x = 0; pos.x < store->size.x; pos.x++) {
    for(pos.y = 0; pos.y < store->size.y; pos.y++) {
      for(pos.z = 0; pos.z < store->size.z; pos.z++) {
        (*store)[pos] = BlockType::Air;
      }
    }
  }

  // top cap
  (*store)[{2, 6, 2}] = BlockType::Leaf;
  (*store)[{3, 6, 2}] = BlockType::Leaf;
  (*store)[{2, 6, 3}] = BlockType::Leaf;
  (*store)[{1, 6, 2}] = BlockType::Leaf;
  (*store)[{2, 6, 1}] = BlockType::Leaf;

  // top
  pos = ivec3(0, 5, 0);
  for(pos.x = 1; pos.x < store->size.x -1; pos.x++) {
    for(pos.z = 1; pos.z < store->size.z -1; pos.z++) {
      (*store)[pos] = BlockType::Leaf;
    }
  }

  // body
  for(pos.x = 0; pos.x < store->size.x; pos.x++) {
    for(pos.y = 3; pos.y < 5; pos.y++) {
      for(pos.z = 0; pos.z < store->size.z; pos.z++) {
      (*store)[pos] = BlockType::Leaf;
      }
    }
  }

  // trunk
  pos = ivec3(2, 1, 2);
  for(pos.y = 0; pos.y < 4; pos.y++) {
    (*store)[pos] = BlockType::Wood;
  }
}

#include "debug/Debug.hpp"

std::vector<Tree::Slice> Tree::spawn(Chunk& chunk, ivec3 pos) const {
  std::vector<Tree::Slice> slices;
  ivec3 origin(2, -1, 2); // the tree is generated centered on the trunk, one block above
  ivec3 start = pos - origin;

  auto genSlice = [&](ivec3 chunkOffset) {
    ivec3 sliceStart = start - chunk.size * chunkOffset; // position of the structure's min corner in this chunk space
    slices.emplace_back(Tree::Slice{
      store,
      chunk.chunkPos + chunkOffset,
      max(sliceStart, ivec3(0)) - sliceStart,
      max(sliceStart, ivec3(0)),
      min(sliceStart + store->size, chunk.size)
    });
  };

  auto minCorner = -ivec3(lessThan(start, ivec3(0)));
  auto maxCorner = ivec3(greaterThanEqual(start + store->size, chunk.size));
  ivec3 dpos;
  for(dpos.x = minCorner.x; dpos.x <= maxCorner.x; dpos.x++) {
    for(dpos.y = minCorner.y; dpos.y <= maxCorner.y; dpos.y++) {
      for(dpos.z = minCorner.z; dpos.z <= maxCorner.z; dpos.z++) {
        genSlice(dpos);
      }
    }
  }

  return slices;
}

void Structure::applySlice(Chunk& chunk, Slice const& slice) {
  ivec3 dpos;
  for(dpos.x = slice.start.x; dpos.x < slice.end.x; dpos.x++) {
    for(dpos.y = slice.start.y; dpos.y < slice.end.y; dpos.y++) {
      for(dpos.z = slice.start.z; dpos.z < slice.end.z; dpos.z++) {
        auto blockType = (*slice.store)[slice.off + dpos - slice.start];
        if(blockType != BlockType::Air)
          chunk[dpos] = AllBlocks::create_static(blockType);
      }
    }
  }
}
