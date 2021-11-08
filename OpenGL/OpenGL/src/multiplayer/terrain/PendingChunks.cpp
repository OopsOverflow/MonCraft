#include "PendingChunks.hpp"

#include <algorithm>
#include <chrono>
#include <limits>
#include <memory>
#include <glm/glm.hpp>

#include "save/ServerConfig.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"

using namespace glm;

PendingChunks::PendingChunks()
  : cpos(std::numeric_limits<int>::max()),
    hasChanged(false),
    world(World::getInst())
{
  auto& config = Config::getServerConfig();
  renderDistH = config.renderDistH;
  renderDistV = config.renderDistV;
  maxChunks = renderDistH * renderDistH * renderDistV;
}

PendingChunks::~PendingChunks() {
  if(thread.get_id() != std::thread::id())
    thread.join();
}

void PendingChunks::update(vec3 playerPos) {
  ivec3 newChunkPos = floor(playerPos / float(chunkSize));
  if(glm::length(vec3(newChunkPos - cpos)) > 2) {
    cpos = newChunkPos;
    updateWaitingChunks();
  }
}

bool PendingChunks::changed() {
  std::lock_guard<std::mutex> lck(copyMutex);
  return hasChanged;
}

std::vector<glm::ivec3> PendingChunks::get() {
  std::lock_guard<std::mutex> lck(copyMutex);
  hasChanged = false;
  if(tcpos != cpos) updateWaitingChunks();
  return waitingChunks;
}

void PendingChunks::updateWaitingChunks() {
  if(running.try_lock()) {
    if(thread.get_id() != std::thread::id()) {
      thread.join();
    }
    tcpos = cpos;
    thread = std::thread(&PendingChunks::updateWorker, this);
  }
}

void PendingChunks::updateWorker() {
  newWaitingChunks.clear();

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = tcpos + ivec3(pos.x, i, pos.y);
      auto chunk = world.chunks.find(p);
      if(!chunk) {
        newWaitingChunks.push_back(p);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

  {
    std::lock_guard<std::mutex> lck(copyMutex);
    waitingChunks = newWaitingChunks;
    hasChanged = true;
  }

  running.unlock();
}

void PendingChunks::remOldChunks() {
  int delCount = std::max<int>((unsigned int)world.chunks.size() - maxChunks, 0);
  world.chunks.eraseChunks(delCount, [=](ivec3 thisChunkPos) {
    ivec3 dist = abs(cpos - thisChunkPos);
    return dist.x > renderDistH + 1 || dist.z > renderDistH + 1 || dist.y > renderDistV + 1;
  });
}
