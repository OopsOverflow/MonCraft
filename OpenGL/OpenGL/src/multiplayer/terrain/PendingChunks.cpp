#include "PendingChunks.hpp"

using namespace glm;

PendingChunks::PendingChunks()
  : world(World::getInst())
{
  updateWaitingChunks();
}

bool PendingChunks::changed(vec3 playerPos) {
  ivec3 newChunkPos = floor(playerPos / float(chunkSize));
  if(newChunkPos != cpos) {
    cpos = newChunkPos;
    updateWaitingChunks();
    return true;
  }

  return false;
}

std::vector<glm::ivec3> PendingChunks::get() {
  return waitingChunks;
}

void PendingChunks::updateWaitingChunks() {
  waitingChunks.clear();

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = cpos + ivec3(pos.x, i, pos.y);
      auto chunk = world.chunks.find(p);
      if(!chunk || !chunk->isLoaded()) {
        waitingChunks.push_back(p);
      }
    }
  };

  insert({0, 0});

  for(int dist = 1; dist < renderDistH+1; dist++) {
    insert({dist, 0});
    insert({0, dist});
    insert({-dist, 0});
    insert({0, -dist});

    for(int off = 1; off < dist; off++) {
      insert({dist, off});
      insert({off, dist});
      insert({-off, dist});
      insert({-dist, off});
      insert({-dist, -off});
      insert({-off, -dist});
      insert({off, -dist});
      insert({dist, -off});
    }

    insert({dist, dist});
    insert({-dist, dist});
    insert({-dist, -dist});
    insert({dist, -dist});
  }
}
