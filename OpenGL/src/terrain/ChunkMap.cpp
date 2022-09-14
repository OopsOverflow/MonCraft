#include "ChunkMap.hpp"

#include "debug/Debug.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <utility>

ChunkMap::ChunkMap()
{}

ChunkMap::ChunkMap(ChunkMap const& chunkMap) {
  std::lock_guard<std::mutex> lck(chunkMap.chunksMutex);
  this->chunks = chunkMap.chunks;
}

ChunkMap& ChunkMap::operator=(ChunkMap const& chunkMap) {
  std::lock_guard<std::mutex> lck(chunkMap.chunksMutex);
  std::lock_guard<std::mutex> lck2(this->chunksMutex);
  this->chunks = chunkMap.chunks;
  return *this;
}

std::shared_ptr<ChunkImpl> ChunkMap::find(glm::ivec3 cpos) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  auto it = chunks.find(cpos);
  if(it == chunks.end()) return nullptr;
  return it->second;
}

std::shared_ptr<ChunkImpl> ChunkMap::insert(glm::ivec3 cpos, std::unique_ptr<ChunkImpl> chunk) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  auto res = chunks.emplace(cpos, std::move(chunk));
  if(!res.second) {
    spdlog::warn("ChunkMap::insert: chunk already existed: {}", cpos);
  }
  return res.first->second;
}

size_t ChunkMap::size() {
  return chunks.size();
}

void ChunkMap::eraseChunks(int count, std::function<bool(ChunkImpl*)> predicate) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  for(auto it = chunks.begin(); it != chunks.end() && count; ) {
    if(predicate(it->second.get())) {
      count --;
      auto useCount = it->second.use_count();
      if(useCount > 1) {
        spdlog::warn("Dangerous chunk erase prevented: uses={}, pos={}", it->second.use_count(), it->first);
        ++it;
      }
      else it = chunks.erase(it);
    }
    else ++it;
  }
}

void ChunkMap::for_each(std::function<void(std::shared_ptr<ChunkImpl>)> callback) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  for(auto const& pair : chunks) {
    if(pair.second)
      callback(pair.second);
  }
}
