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
using namespace std::chrono_literals;

#include "debug/Debug.hpp"

PendingChunks::PendingChunks()
  : cpos(std::numeric_limits<int>::max()),
    tcpos(cpos),
    hasChanged(false),
    world(World::getInst()),
    stopFlag(false)
{
  auto& config = Config::getServerConfig();
  renderDistH = config.renderDistH;
  renderDistV = config.renderDistV;
  maxChunks = renderDistH * renderDistH * renderDistV;

  thread = std::thread(&PendingChunks::updateWorker, this);
}

PendingChunks::~PendingChunks() {
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = true;
  }
  stopSignal.notify_all();
  thread.join();
}

void PendingChunks::update(vec3 playerPos) {
  ivec3 newChunkPos = floor(playerPos / float(chunkSize));
  if(newChunkPos != cpos) {
    std::lock_guard<std::mutex> lck(copyMutex);
    cpos = newChunkPos;
  }
}

bool PendingChunks::changed() {
  std::lock_guard<std::mutex> lck(copyMutex);
  return hasChanged;
}

std::vector<glm::ivec3> PendingChunks::get() {
  std::lock_guard<std::mutex> lck(copyMutex);
  hasChanged = false;
  return waitingChunks;
}

void PendingChunks::updateWaitingChunks() {
  std::vector<glm::ivec3> newWaitingChunks;

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = tcpos + ivec3(pos.x, i, pos.y);
      auto chunk = world.chunks.find(p);
      if(!chunk) {
        newWaitingChunks.push_back(p);
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

  {
    std::lock_guard<std::mutex> lck(copyMutex);
    waitingChunks = newWaitingChunks;
    hasChanged = true;
  }
}

bool PendingChunks::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  auto res = stopSignal.wait_for(stopLck, millis, [&]{return stopFlag;});
  return res;
}

void PendingChunks::updateWorker() {
  static const auto sleep = 500ms;
  bool mustUpdate = false;

  do {
    {
      std::lock_guard<std::mutex> lck(copyMutex);
      mustUpdate = cpos != tcpos;
      tcpos = cpos;
    }
    if(mustUpdate) {
      updateWaitingChunks();
    }
  } while(!sleepFor(sleep));
}

void PendingChunks::remOldChunks() {
  int delCount = std::max<int>((unsigned int)world.chunks.size() - maxChunks, 0);
  world.chunks.eraseChunks(delCount, [=](ivec3 thisChunkPos) {
    ivec3 dist = abs(cpos - thisChunkPos);
    return dist.x > renderDistH + 1 || dist.z > renderDistH + 1 || dist.y > renderDistV + 1;
  });
}
