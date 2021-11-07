#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include <functional>

#include "AbstractChunk.hpp"

/**
 * Stores all the chunks in a hashmap.
 * This is mostly thread-safe.
 */

class ChunkMap {

public:
  ChunkMap();

  /**
   * Finds a chunks at a given chunk index.
   * Returns nullptr if the chunks is not in the hashmap.
   */
  std::shared_ptr<AbstractChunk> find(glm::ivec3 cpos);

  /**
   * Inserts a new chunk in the hashmap at the given chunk index.
   * returns the chunk contained at the chunk index after inserting the chunk.
   * /!\ if a chunk already exists, it will drop the proposed chunk and return
   * the one already in place.
   */
  std::shared_ptr<AbstractChunk> insert(glm::ivec3 cpos, std::unique_ptr<AbstractChunk>);


  /**
   * Erase all the chunks meeting the requirements of the predicate, limited
   * to the count provided.
   * the predicate must return true if the chunk is to be deleted. It takes
   * the chunk index of a given stored chunk.
   */
  void eraseChunks(int count, std::function<bool(glm::ivec3)> predicate);

  /**
   * Analogous to std::unordered_map.size().
   */
  size_t size();

  /**
   * Calls the callback with each stored chunk.
   */
  void for_each(std::function<void(std::shared_ptr<AbstractChunk>)> callback);

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
  std::unordered_map<glm::ivec3, std::shared_ptr<AbstractChunk>, ivec3_hash, ivec3_hash> chunks;
};
