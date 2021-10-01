#include "debug/Debug.hpp"
#include "multiplayer/server/UdpServer.hpp"
#include "multiplayer/server/WebSocketServer.hpp"
#include "multiplayer/common/Config.hpp"
#include "util/SaveManager.hpp"


std::unique_ptr<Server> make_server() {
  auto port = SaveManager::getInst().getConfig().serverPort;
  return std::make_unique<UdpServer>(port);
  // return std::make_unique<WebSocketServer>(port);
}

int main() {
  std::cout << "---- Server ----" << std::endl;
  Config config = SaveManager::getInst().getConfig();

  // game seed
  std::hash<std::string> hashString;
  auto seed = hashString(config.seed);
  std::srand(seed);
  std::cout << "seed : " << config.seed << " (" << seed << ")" << std::endl;

  SaveManager::configSaveDir = "save";
  SaveManager::configFilename = "serverConfig.txt";
  SaveManager::entitySaveDir = "save/serverWorld/entities";
  SaveManager::chunkSaveDir = "save/serverWorld/chunks";
  std::unique_ptr<Server> server = make_server();
  server->run();
  return 0;
}
