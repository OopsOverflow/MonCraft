#pragma once
#include <glm/glm.hpp>
#include <string>

namespace NetworkConfig {
  static const unsigned short PACKET_SIZE = 100;
  static const unsigned short SERVER_TICK = 500; // millis
  static const glm::vec3 SPAWN_POINT = glm::vec3(0.0f, 100.0f, 0.0f);
};
