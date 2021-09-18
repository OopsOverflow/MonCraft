#pragma once

#include <glm/glm.hpp>
#include "terrain/Chunk.hpp"

class SaveManager
{
public:
    SaveManager(std::string save_path);
    ~SaveManager();

    bool isStored(glm::ivec3 chunkPos);
    std::unique_ptr<Chunk> getChunk(glm::ivec3 chunkPos);
    bool saveChunk(Chunk chunk);

private:
    std::string save_path;
    std::fstream openedFile;
};
