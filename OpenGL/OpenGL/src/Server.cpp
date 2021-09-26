#include "debug/Debug.hpp"
#include "multiplayer/server/Server.hpp"
#include "multiplayer/common/Config.hpp"
#include "save/SaveManager.hpp"

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
  Server server(SaveManager::getInst().getConfig().serverPort);
  server.run();
  return 0;
}
