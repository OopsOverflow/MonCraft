#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Config {

    struct ServerConfig {
        ServerConfig();
        ~ServerConfig();

        #ifdef EMSCRIPTEN
          unsigned short serverPort = 55001;
          unsigned int threadCount = 2;
        #else
          unsigned short serverPort = 55000;
          unsigned int threadCount = 8;
        #endif

        std::string seed = "MonCraft";
        unsigned int renderDistH = 10;
        unsigned int renderDistV = 5;
        unsigned short packetSize = 100;
        unsigned short serverTick = 10; // millis
        unsigned short webSocketServerTick = 50; // millis
        glm::vec3 spawnPoint = glm::vec3(0.0f, 15.0f, 0.0f);
    };

    ServerConfig& getServerConfig();
}
