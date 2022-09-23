#include "SliceMap.hpp"

#include <utility>

#include "multiplayer/terrain/Structure.hpp"

SliceMap::SliceMap() {}

void SliceMap::insert(std::vector<Structure::Slice> const& slices) {
  std::lock_guard<std::mutex> lck(slicesMutex);
  for(auto slice : slices) {
    auto& dest = map[slice.targetCpos];
    dest.push_back(slice);
  }
}

Structure::slices_t SliceMap::pop(glm::ivec3 cpos) {
  std::lock_guard<std::mutex> lck(slicesMutex);
  std::vector<Structure::Slice> slices = {};
  auto it = map.find(cpos);
  if(it != map.end()) {
    it->second.swap(slices);
    map.erase(it);
  }
  return slices;
}
