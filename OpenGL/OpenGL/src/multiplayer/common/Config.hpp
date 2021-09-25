#pragma once
#include <glm/glm.hpp>
#include <string>

namespace NetworkConfig {
  static const bool LOCAL = true;
  static const std::string SERVER_ADDR = "pi.thissma.fr";
  static const unsigned short SERVER_PORT = 55000;
  static const unsigned short PACKET_SIZE = 100;
  static const unsigned short SERVER_TICK = 10; // millis
  static const glm::vec3 SPAWN_POINT = glm::vec3(0.0f, 40.0f, 0.0f);
};
