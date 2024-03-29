#pragma once

#include <string>
#include <SDL2/SDL_keycode.h>
#include "ui/Key.hpp"


namespace Config {
    struct ClientConfig {
        ClientConfig();
        ~ClientConfig();

        std::string serverAddr = "pi.thissma.fr";
        unsigned short serverPort = 55000;
        bool serverTLS = true;

        #ifdef EMSCRIPTEN
        bool multiplayer = true;
        unsigned int msaa = 0;
        #else
        bool multiplayer = false;
        unsigned int msaa = 4;
        #endif

        float fov = 70.0f;
        float mainVolume = 50.0f;
        float musicVolume = 50.0f;
        float sensivity = 50.0f; // rotation radians per mouse pixel on screen
        bool fullscreen = false;
        bool vsync = true;
        int shadows = 2;
        Key forward = SDLK_z;
        Key backward = SDLK_s;
        Key left = SDLK_q;
        Key right = SDLK_d;
        Key jump = SDLK_SPACE;
        Key sneak = SDLK_LSHIFT;
        Key view = SDLK_F5;
        Key sprint = SDLK_LCTRL;
        Key menu = SDLK_ESCAPE;
        Key dab = SDLK_t;
    };

    ClientConfig& getClientConfig();
}
