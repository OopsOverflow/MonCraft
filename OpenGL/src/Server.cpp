#include "multiplayer/server/Server.hpp"

#include <iostream>
#include <memory>
#include <string>

// #include "multiplayer/server/UdpServer.hpp"
#include "multiplayer/server/WebSocketServer.hpp"
#include "noise/prng.hpp"
#include "save/SaveManager.hpp"
#include "save/ServerConfig.hpp"


std::unique_ptr<Server> make_server() {
  auto& config = Config::getServerConfig();
  return std::make_unique<WebSocketServer>(config.serverPort, config.serverTLS);
  // return std::make_unique<UdpServer>(port);
}

int main() {
  std::cout << "---- Server ----" << std::endl;
  auto& config = Config::getServerConfig();

  // game seed
  auto seed = prng::srands(config.seed);
  std::cout << "seed : " << config.seed << " (" << seed << ")" << std::endl;

  SaveManager::configSaveDir = "save";
  SaveManager::entitySaveDir = "save/serverWorld/entities";
  SaveManager::chunkSaveDir = "save/serverWorld/chunks";
  std::unique_ptr<Server> server = make_server();
  server->run();
  return 0;
}
