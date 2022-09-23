#include "Server.hpp"

#include <iostream>
#include <spdlog/spdlog.h>

#include "terrain/World.hpp"
#include "terrain/ChunkImpl.hpp"
#include "save/ServerConfig.hpp"

Server::Server()
  : state(ServerState::DISCONNECTED)
{
  auto& config = Config::getServerConfig();
  renderDistH = config.renderDistH;
  renderDistV = config.renderDistV;
  maxChunks = renderDistH * renderDistH * renderDistV;
}

Server::~Server() {
  stop();
}

void Server::update() {
  remOldChunks();
}

void Server::remOldChunks() {
  World& world = World::getInst();
  glm::ivec3 cpos = floor(getPlayer()->getPosition() / 16.f);
  int delCount = std::max<int>((unsigned int)world.chunks.size() - maxChunks, 0);
  world.chunks.eraseChunks(delCount, [=, this](Chunk* chunk) {
    glm::ivec3 dist = abs(cpos - chunk->chunkPos);
    return dist.x > renderDistH + 1 || dist.z > renderDistH + 1 || dist.y > renderDistV + 1;
  });
}


bool Server::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  return stopSignal.wait_for(stopLck, millis, [&]{ return stopFlag; });
}

void Server::loop() {
  bool stop = false;
  auto sleep = std::chrono::milliseconds(Config::getServerConfig().serverTick);

  while(!stop) {
    update();
    stop = sleepFor(sleep);
  }
}

void Server::start() {
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = false;
  }
  serverThread = std::thread(&Server::loop, this);
}

void Server::stop() {
  spdlog::info("Shutting down local server...");
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = true;
  }
  stopSignal.notify_all();
  if(serverThread.joinable()) serverThread.join();
  spdlog::info("Local server terminated");
}
