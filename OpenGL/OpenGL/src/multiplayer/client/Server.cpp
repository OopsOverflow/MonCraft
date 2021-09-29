#include "Server.hpp"
#include "multiplayer/common/Config.hpp"


Server::Server()
{}

Server::~Server() {
  stop();
}

bool Server::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  return stopSignal.wait_for(stopLck, millis, [&]{ return stopFlag; });
}

void Server::loop() {
  bool stop = false;
  auto sleep = std::chrono::milliseconds(NetworkConfig::SERVER_TICK);
  bool logged = false;

  while(!stop && !logged) {
    logged = login();
    stop = sleepFor(sleep);
  }

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
  std::cout << "shutting down server..." << std::endl;
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = true;
  }
  stopSignal.notify_all();
  if(serverThread.joinable()) serverThread.join();
}
