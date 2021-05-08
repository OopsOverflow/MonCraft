#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include <functional>

#include "Chunk.hpp"

class ChunkMap {

public:
  ChunkMap();

  std::shared_ptr<Chunk> find(glm::ivec3 cpos);

  std::shared_ptr<Chunk> insert(glm::ivec3 cpos, std::unique_ptr<Chunk>);

  void eraseChunks(int count, std::function<bool(glm::ivec3)> predicate);

  void for_each(std::function<void(std::shared_ptr<Chunk>)> callback);

private:

  struct ivec3_hash
  {
    size_t operator()(glm::ivec3 const& k) const {
      return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^  std::hash<int>()(k.z);
    }
    bool operator()(glm::ivec3 const& a, glm::ivec3 const& b) const {
      return a == b;
    }
  };

  std::mutex chunksMutex; // serializes access to the hashmap
  std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, ivec3_hash, ivec3_hash> chunks;
};
