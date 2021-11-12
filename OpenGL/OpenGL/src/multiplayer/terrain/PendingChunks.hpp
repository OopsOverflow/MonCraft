#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <glm/glm.hpp>

class World;

class PendingChunks {

public:
  PendingChunks();
  ~PendingChunks();

  void update(glm::vec3 playerPos);
  bool changed();

  std::vector<glm::ivec3> get();

  void remOldChunks();

private:
  bool sleepFor(std::chrono::milliseconds);
  void updateWorker();
  void updateWaitingChunks();

  static const int chunkSize = 16; // TODO: in a config file
  int renderDistH, renderDistV;
  unsigned int maxChunks;
  glm::ivec3 cpos, tcpos;
  bool hasChanged;
  std::vector<glm::ivec3> waitingChunks;
  std::mutex copyMutex;
  std::thread thread;
  World& world;

  // signals to stop the threads
  bool stopFlag;
  std::mutex stopMutex;
  std::condition_variable stopSignal;
};
