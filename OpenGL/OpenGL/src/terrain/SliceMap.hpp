#pragma once

#include <unordered_map>

#include "Structure.hpp"

class SliceMap {

public:
  // ~SliceMap();
  SliceMap();

  void insert(std::vector<Structure::Slice> const& slices);

  std::vector<Structure::Slice> pop(glm::ivec3 cpos);

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
