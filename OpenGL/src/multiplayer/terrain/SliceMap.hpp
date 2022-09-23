#pragma once

#include <glm/glm.hpp>
#include <stddef.h>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "terrain/Structure.hpp"

/**
 * Stores all the slices to be later applied to chunks.
 */
class SliceMap {

public:
  SliceMap();

  /**
   * Inserts slices in the map.
   */
  void insert(std::vector<Structure::Slice> const& slices);

  /**
   * Gives all the slices stored for a given chunk index and removes the entry.
   */
  Structure::slices_t pop(glm::ivec3 cpos);

  template<typename Fn>
  Structure::slices_t pop_if(glm::ivec3 cpos, Fn predicate) {
    std::lock_guard<std::mutex> lck(slicesMutex);
    auto it = map.find(cpos);
    if(it != map.end()) {
      auto end = std::remove_if(it->second.begin(), it->second.end(), predicate);
      Structure::slices_t slices(end, it->second.end());
      if(end == it->second.begin()) { // no slices left
        map.erase(it);
      }
      else {
        it->second.erase(end, it->second.end());
      }
      return slices;
    }
    else {
      return {};
    }
  }

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

  std::mutex slicesMutex;
  std::unordered_map<glm::ivec3, std::vector<Structure::Slice>, ivec3_hash, ivec3_hash> map;
};
