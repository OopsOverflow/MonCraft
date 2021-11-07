#include "BlockArray.hpp"
#include "blocks/AllBlocks.hpp"
#include "terrain/World.hpp"
#include <sstream>
#include <set>

std::vector<glm::ivec3> BlockArray::getChangedChunks() const {
  float chunkSize = World::getInst().chunkSize; // TODO: in config
  std::vector<glm::ivec3> res;
  for(auto const& elt : *this) {
    glm::ivec3 cpos = floor(glm::vec3(elt.pos) / chunkSize);
    if(std::find(res.begin(), res.end(), cpos) == res.end())
      res.push_back(cpos);
  }
  return res;
}

void BlockArray::push(glm::ivec3 pos, Block* block) {
  std::ostringstream ss;
  AllBlocks::serialize(ss, block);
  push_back({ pos, ss.str() });
}

void BlockArray::copyToWorld() {
  World& world = World::getInst();

  for(auto const& elt : *this) {
    std::istringstream ss(elt.ser);
    world.setBlock(elt.pos, AllBlocks::deserialize(ss));
    Block* block = world.getBlock(elt.pos);
    std::cout << (int)block->type << std::endl;
  }
}
