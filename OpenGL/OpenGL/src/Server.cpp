#include "debug/Debug.hpp"
#include "multiplayer/server/Server.hpp"
#include "multiplayer/common/Config.hpp"
#include "save/SaveManager.hpp"

int main() {
  std::cout << "---- Server ----" << std::endl;
  Server server(SaveManager::getInst().getConfig().serverPort);
  server.run();
  return 0;
}
