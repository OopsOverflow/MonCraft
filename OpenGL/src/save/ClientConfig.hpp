#pragma once

#include <string>
#include <SDL2/SDL_keycode.h>


namespace Config {
    struct ClientConfig {
        ClientConfig();
        ~ClientConfig();

        std::string serverAddr = "pi.thissma.fr";

        #ifdef EMSCRIPTEN
        bool multiplayer = true;
        unsigned short serverPort = 55001;
        unsigned int msaa = 0;
        #else
        bool multiplayer = false;
        unsigned short serverPort = 55000;
        unsigned int msaa = 4;
        #endif

        float fov = 70.0;
        SDL_Keycode forward = SDLK_z;
        SDL_Keycode backward = SDLK_s;
        SDL_Keycode left = SDLK_q;
        SDL_Keycode right = SDLK_d;
        SDL_Keycode jump = SDLK_SPACE;
        SDL_Keycode sneak = SDLK_LSHIFT;
        SDL_Keycode view = SDLK_F5;
        SDL_Keycode sprint = SDLK_LCTRL;
        SDL_Keycode menu = SDLK_ESCAPE;
    };

    ClientConfig& getClientConfig();
}
