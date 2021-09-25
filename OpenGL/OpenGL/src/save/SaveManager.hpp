#pragma once

#include <glm/glm.hpp>
#include "terrain/Chunk.hpp"
#include "multiplayer/common/Identifier.hpp"

class Entity;

enum class EntityClass{Character};

class SaveManager
{
public:
    SaveManager(std::string save_path);
    SaveManager(SaveManager const&) = delete;
    SaveManager& operator=(SaveManager const&) = delete;

    std::unique_ptr<Chunk> getChunk(glm::ivec3 chunkPos);
    bool saveChunk(std::shared_ptr<Chunk> chunk);

    std::unique_ptr<Entity> getEntity(Identifier uid);
    bool saveEntity(const Entity& entity);

private:
    std::string save_path;
};
