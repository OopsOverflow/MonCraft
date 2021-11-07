#include "PendingChunks.hpp"
#include "util/SaveManager.hpp"

using namespace glm;

PendingChunks::PendingChunks()
  : cpos(std::numeric_limits<int>::max()),
    world(World::getInst())
{
  auto& config = SaveManager::getInst().getConfig();
  renderDistH = config.renderDistH;
  renderDistV = config.renderDistV;
  maxChunks = renderDistH * renderDistH * renderDistV;
  updateWaitingChunks();
}

#include "debug/Debug.hpp"

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
  std::lock_guard<std::mutex> lck(updateMutex);
  return waitingChunks;
}

void PendingChunks::updateWaitingChunks() {
  if(!updateMutex.try_lock()) {
    std::cout << "[WARN] already updating waiting chunks" << std::endl;
    return;
  }
  waitingChunks.clear();

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = cpos + ivec3(pos.x, i, pos.y);
      auto chunk = world.chunks.find(p);
      if(!chunk || !chunk->isComputed()) {
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
  std::cout << "[INFO] waiting chunks : " << waitingChunks.size() << std::endl;
  updateMutex.unlock();
}

void PendingChunks::remOldChunks() {
  int delCount = std::max<int>((unsigned int)world.chunks.size() - maxChunks, 0);
  world.chunks.eraseChunks(delCount, [=](ivec3 thisChunkPos) {
    ivec3 dist = abs(cpos - thisChunkPos);
    return dist.x > renderDistH + 1 || dist.z > renderDistH + 1 || dist.y > renderDistV + 1;
  });
}
