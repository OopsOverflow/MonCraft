#include "World.hpp"

#include <glm/glm.hpp>
#include <chrono>
#include <memory>
#include <utility>

#include "terrain/ChunkImpl.hpp"
#include "terrain/ChunkMap.hpp"

using namespace glm;
using namespace std::chrono_literals;

World& World::getInst() {
  static World world;
  return world;
}

World::World() : t(8.f * dayDuration / 24.f)
{}

World::~World()
{}

Block* World::getBlock(ivec3 pos) {
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  if(auto chunk = chunks.find(cpos)) {
    ivec3 dpos = pos - cpos * chunkSize;
    return chunk->at(dpos).get();
  }

  return nullptr;
}

bool World::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  auto chunk = chunks.find(cpos);
  if(!chunk) return false;

  ivec3 dpos = pos - cpos * chunkSize;
  chunk->setBlock(dpos, std::move(block));
  return true;
}
