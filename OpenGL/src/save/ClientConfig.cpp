#include "ClientConfig.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>

#include "save/SaveManager.hpp"
#include "util/Serde.hpp"

Config::ClientConfig::ClientConfig() {
  const std::string path = SaveManager::configSaveDir + "/clientConfig.txt";
  std::ifstream openedFile(path);
  if (!openedFile.is_open()) return;

  std::string readLine;
  std::string param;

#define KEY_PARAM(NAME)                                                        \
  if (param == #NAME ":") {                                                    \
    this->NAME = SDL_GetKeyFromName(serde::remainder(ss).c_str());             \
    continue;                                                                  \
  }
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

    PARAM(serverAddr)
    PARAM(serverPort)
    PARAM(serverTLS)
    PARAM(fov)
    PARAM(msaa)
    KEY_PARAM(forward)
    KEY_PARAM(backward)
    KEY_PARAM(left)
    KEY_PARAM(right)
    KEY_PARAM(jump)
    KEY_PARAM(sneak)
    KEY_PARAM(view)
    KEY_PARAM(menu)
    KEY_PARAM(dab)
    PARAM(mainVolume)
    PARAM(musicVolume)
    PARAM(fullscreen)
    PARAM(vsync)
    PARAM(shadows)
    PARAM(sensivity)

  }

  #undef KEY_PARAM
  #undef PARAM

  fov = std::min(180.0f, std::max(0.0f, fov));
}

Config::ClientConfig::~ClientConfig() {
  const std::string path = SaveManager::configSaveDir + "/clientConfig.txt";
  std::filesystem::create_directories(SaveManager::configSaveDir);
  std::ofstream openedFile(path, std::fstream::trunc);
  if (!openedFile) {
    spdlog::warn("Failed to open client config file: ''", path);
    return;
  }
  openedFile << std::boolalpha << "MonCraft v1.1.0" << std::endl;

  #define KEY_PARAM(NAME)                                                      \
    openedFile << "\t" #NAME ": " << SDL_GetKeyName(this->NAME.asKeycode()) << std::endl;
  #define PARAM(NAME)                                                          \
    openedFile << "\t" #NAME ": " << this->NAME << std::endl;

  PARAM(serverAddr)
  PARAM(serverPort)
  PARAM(serverTLS)
  PARAM(fov)
  PARAM(msaa)
  KEY_PARAM(forward)
  KEY_PARAM(backward)
  KEY_PARAM(left)
  KEY_PARAM(right)
  KEY_PARAM(jump)
  KEY_PARAM(sneak)
  KEY_PARAM(view)
  KEY_PARAM(menu)
  KEY_PARAM(dab)
  PARAM(mainVolume)
  PARAM(musicVolume)
  PARAM(fullscreen)
  PARAM(vsync)
  PARAM(shadows)
  PARAM(sensivity)

  #undef KEY_PARAM
  #undef PARAM
}

Config::ClientConfig& Config::getClientConfig() {
  static ClientConfig config;
  return config;
}
