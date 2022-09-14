#include "ServerConfig.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "save/SaveManager.hpp"


Config::ServerConfig::ServerConfig() {
  const std::string path = SaveManager::configSaveDir + "/serverConfig.txt";
  std::ifstream openedFile(path);
  if (!openedFile.is_open()) return;

  std::string readLine;
  std::string param;

#define PARAM(NAME)                                                            \
  if (param == #NAME ":") {                                                    \
    ss >> this->NAME;                                                          \
    continue;                                                                  \
  }

  while (getline(openedFile, readLine)) {
    std::stringstream ss;
    ss >> std::boolalpha;
    ss.str(readLine);
    ss >> param;

    PARAM(serverPort)
    PARAM(serverTLS)
    PARAM(seed)
    PARAM(threadCount)
    PARAM(renderDistH)
    PARAM(renderDistV)
    PARAM(packetSize)
    PARAM(serverTick)
    PARAM(webSocketServerTick)
  }

  #undef PARAM
}

Config::ServerConfig::~ServerConfig() {
  const std::string path = SaveManager::configSaveDir + "/serverConfig.txt";
  std::filesystem::create_directories(SaveManager::configSaveDir);
  std::ofstream openedFile(path, std::fstream::trunc);
  if (!openedFile) {
    std::cout << "[WARN] failed to open file: " << path << std::endl;
    return;
  }
  openedFile << std::boolalpha << "MonCraft v1.1.0" << std::endl;

  #define PARAM(NAME)                                                          \
    openedFile << "\t" #NAME ": " << this->NAME << std::endl;

  PARAM(serverPort)
  PARAM(serverTLS)
  PARAM(seed)
  PARAM(threadCount)
  PARAM(renderDistH)
  PARAM(renderDistV)
  PARAM(packetSize)
  PARAM(serverTick)
  PARAM(webSocketServerTick)
  #undef PARAM
}

Config::ServerConfig& Config::getServerConfig() {
  static ServerConfig config;
  return config;
}
