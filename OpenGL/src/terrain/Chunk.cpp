#include "Chunk.hpp"

#include <glm/glm.hpp>
#include <algorithm>
#include <utility>

using namespace glm;
using std::move;

const std::array<ivec3, 26> Chunk::neighborOffsets = {
  // ivec3{ 0, 0, 0 },
  ivec3{ 0, 0, 1 }, // 0
  ivec3{ 0, 0,-1 }, // 1
  ivec3{ 0, 1, 0 }, // 2
  ivec3{ 0, 1, 1 }, // 3
  ivec3{ 0, 1,-1 }, // 4
  ivec3{ 0,-1, 0 }, // 5
  ivec3{ 0,-1, 1 }, // 6
  ivec3{ 0,-1,-1 }, // 7

  ivec3{ 1, 0, 0 }, // 8
  ivec3{ 1, 0, 1 }, // 9
  ivec3{ 1, 0,-1 }, // 10
  ivec3{ 1, 1, 0 }, // 11
  ivec3{ 1, 1, 1 }, // 12
  ivec3{ 1, 1,-1 }, // 13
  ivec3{ 1,-1, 0 }, // 14
  ivec3{ 1,-1, 1 }, // 15
  ivec3{ 1,-1,-1 }, // 16

  ivec3{-1, 0, 0 }, // 17
  ivec3{-1, 0, 1 }, // 18
  ivec3{-1, 0,-1 }, // 19
  ivec3{-1, 1, 0 }, // 20
  ivec3{-1, 1, 1 }, // 21
  ivec3{-1, 1,-1 }, // 22
  ivec3{-1,-1, 0 }, // 23
  ivec3{-1,-1, 1 }, // 24
  ivec3{-1,-1,-1 }, // 25
};

const std::array<int, 26> Chunk::neighborOffsetsInverse = { // https://oeis.org/A059249
  1, 0, 5, 7, 6, 2, 4, 3,
  17, 19, 18, 23, 25, 24, 20, 22, 21,
  8, 10, 9, 14, 16, 15, 11, 13, 12
};

Chunk::Chunk(ivec3 chunkPos, int chunkSize)
  : DataStore<Block::unique_ptr_t, 3>(glm::ivec3(chunkSize)),
    chunkPos(chunkPos), computed(false)
{}

std::weak_ptr<Chunk> Chunk::getNeighbor(glm::ivec3 off) {
  static const ivec3 mask(9, 3, 1);
  auto greater = equal(off, ivec3(1));
  auto lesser = equal(off, ivec3(-1));

  auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
  int index = tmp.x + tmp.y + tmp.z - 1;
  return neighbors[index];
}

Block* Chunk::getBlockAccrossChunks(ivec3 pos) const {
  static const ivec3 mask(9, 3, 1);
  auto greater = greaterThanEqual(pos, size());
  auto lesser = lessThan(pos, ivec3(0));

  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
    int index = tmp.x + tmp.y + tmp.z - 1;

    if(auto neigh = neighbors[index].lock()) {
      ivec3 otherChunkPos = pos - size() * (ivec3(greater) - ivec3(lesser));
      return neigh->at(otherChunkPos).get();
    }
    else return nullptr;
  }
  return (*this)[pos].get();
}

void Chunk::setBlockAccrossChunks(ivec3 pos, Block::unique_ptr_t block) {
  static const ivec3 mask(9, 3, 1);
  auto greater = greaterThanEqual(pos, size());
  auto lesser = lessThan(pos, ivec3(0));

  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
    int index = tmp.x + tmp.y + tmp.z - 1;

    if(auto neigh = neighbors[index].lock()) {
      ivec3 otherChunkPos = pos - size() * (ivec3(greater) - ivec3(lesser));
      return neigh->setBlock(otherChunkPos, std::move(block));
    }
  }
  else setBlock(pos, std::move(block));
}

bool Chunk::hasAllNeighbors() const {
  return std::all_of(neighbors.begin(), neighbors.end(), [](auto neigh) {
    return neigh.lock() != nullptr;
  });
}

bool Chunk::hasNoNeighbors() const {
  return std::all_of(neighbors.begin(), neighbors.end(), [](auto neigh) {
    return neigh.lock() == nullptr;
  });
}

void Chunk::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  at(pos) = std::move(block);
}
