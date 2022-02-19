#pragma once

#include <memory>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <mutex>

#include "util/Identifier.hpp"

class Character;

class Server {

public:
  Server();
  virtual ~Server();

  virtual void ping() = 0;
  virtual void update();
  virtual bool login() = 0;
  void start();
  void stop();

  /**
   * Returns nullptr if the player was not created.
   */
  virtual std::shared_ptr<Character> getPlayer() = 0;
  virtual Identifier getUid() = 0;

private:
  void remOldChunks();
  void loop();
  bool sleepFor(std::chrono::milliseconds);
  std::thread serverThread;

  bool stopFlag;
  std::mutex stopMutex;
  std::condition_variable stopSignal;

  int renderDistH, renderDistV;
  unsigned int maxChunks;
};
