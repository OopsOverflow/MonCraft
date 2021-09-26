#pragma once

#include <glm/glm.hpp>
#include "terrain/Chunk.hpp"
#include "util/Identifier.hpp"
#include <SDL2/SDL_keycode.h>

class Entity;

enum class EntityClass{Character};


struct Config {
    bool multiplayer = false;
    std::string serverAddr = "pi.thissma.fr";
    unsigned short serverPort = 55000;

    unsigned int renderDistH = 10;
    unsigned int renderDistV = 5;
    unsigned int threadCount = 8;

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
    static SaveManager& getInst();
    Config& getConfig();
    bool saveConfig();

    static std::unique_ptr<Chunk> getChunk(glm::ivec3 chunkPos);
    static bool saveChunk(Chunk const& chunk);

    static std::unique_ptr<Entity> getEntity(Identifier uid);
    static bool saveEntity(const Entity& entity);

    static std::string chunkSaveDir, entitySaveDir, configSaveDir;
    static std::string configFilename;

private:
    Config config;
    static Config loadConfig();
    SaveManager();
    ~SaveManager();
    SaveManager(SaveManager const&) = delete;
    SaveManager& operator=(SaveManager const&) = delete;
};

sf::Packet& operator<<(sf::Packet& packet, Chunk const& chunk);
sf::Packet& operator>>(sf::Packet& packet, Chunk& chunk);
