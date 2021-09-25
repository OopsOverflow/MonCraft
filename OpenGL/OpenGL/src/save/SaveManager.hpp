#pragma once

#include <glm/glm.hpp>
#include "terrain/Chunk.hpp"
#include "util/Identifier.hpp"

class Entity;

enum class EntityClass{Character};

class SaveManager
{
public:
    SaveManager(std::string save_path);
    SaveManager(SaveManager const&) = delete;
    SaveManager& operator=(SaveManager const&) = delete;

    std::unique_ptr<Chunk> getChunk(glm::ivec3 chunkPos);
    bool saveChunk(Chunk const& chunk);

    std::unique_ptr<Entity> getEntity(Identifier uid);
    bool saveEntity(const Entity& entity);


private:
    std::string save_path;
};

sf::Packet& operator<<(sf::Packet& packet, Chunk const& chunk);
sf::Packet& operator>>(sf::Packet& packet, Chunk& chunk);
