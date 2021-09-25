#pragma once

#include <glm/glm.hpp>
#include "terrain/Chunk.hpp"
#include "multiplayer/common/Identifier.hpp"
#include <SDL2/include/SDL2/SDL_keycode.h>

class Entity;

enum class EntityClass{Character};


struct Config {
    bool multiplayer = true;
    std::string SERVER_ADDR = "pi.thissma.fr";
    unsigned short SERVER_PORT = 55000;

    float fov = 45.0;
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


class SaveManager
{
public:

    static Config getConfig();
    static bool saveConfig(const Config& config);

    static std::unique_ptr<Chunk> getChunk(glm::ivec3 chunkPos);
    static bool saveChunk(std::shared_ptr<Chunk> chunk);

    static std::unique_ptr<Entity> getEntity(Identifier uid);
    static bool saveEntity(const Entity& entity);

private:
    static std::string chunk_save_path;
    static std::string entity_save_path;

    SaveManager() = delete;
    SaveManager(SaveManager const&) = delete;
    SaveManager& operator=(SaveManager const&) = delete;
};
