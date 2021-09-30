#include "ChunkMap.hpp"


ChunkMap::ChunkMap()
{}

std::shared_ptr<Chunk> ChunkMap::find(glm::ivec3 cpos) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  auto it = chunks.find(cpos);
  if(it == chunks.end()) return nullptr;
  return it->second;
}

#include "debug/Debug.hpp"

std::shared_ptr<Chunk> ChunkMap::insert(glm::ivec3 cpos, std::unique_ptr<Chunk> chunk) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  auto res = chunks.emplace(cpos, std::move(chunk));
  if(!res.second) {
    std::cout << "[WARN] ChunkMap insert: chunk already existed: " << cpos << std::endl;
  }
  return res.first->second;
}

size_t ChunkMap::size() {
  return chunks.size();
}

#include "debug/Debug.hpp"

void ChunkMap::eraseChunks(int count, std::function<bool(glm::ivec3)> predicate) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  for(auto it = chunks.begin(); it != chunks.end() && count; ) {
    if(predicate(it->first)) {
      count --;
      auto useCount = it->second.use_count();
      if(useCount > 1) {
        std::cout << "[WARN] dangerous erase prevented " << it->second.use_count() << " " << it->first << std::endl;
        ++it;
      }
      else it = chunks.erase(it);
    }
    else ++it;
  }
}

void ChunkMap::for_each(std::function<void(std::shared_ptr<Chunk>)> callback) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  for(auto const& pair : chunks) {
    if(pair.second)
      callback(pair.second);
  }
}
