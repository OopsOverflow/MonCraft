#include "debug/Debug.hpp"
#include "multiplayer/server/Server.hpp"
#include "multiplayer/common/Config.hpp"

int main() {
  std::cout << "---- Server ----" << std::endl;
  Server server(NetworkConfig::SERVER_PORT);
  server.run();
  return 0;
}
