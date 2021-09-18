#pragma once

#include <glm/glm.hpp>
#include "terrain/Chunk.hpp"

class SaveManager
{
public:
    SaveManager(std::string save_path);
    SaveManager(SaveManager const&) = delete;
    SaveManager& operator=(SaveManager const&) = delete;

    std::unique_ptr<Chunk> getChunk(glm::ivec3 chunkPos);
    bool saveChunk(std::shared_ptr<Chunk> chunk);

private:
    std::string save_path;
};
