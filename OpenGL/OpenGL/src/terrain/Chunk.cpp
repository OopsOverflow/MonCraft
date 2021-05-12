#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Chunk.hpp"
#include "gl/Shader.hpp"

using namespace glm;
using std::move;

#include "debug/Debug.hpp"

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
    chunkPos(chunkPos),
    solidMesh(nullptr), transparentMesh(nullptr),
    loaded(false), mustRecompute(false)
{
  // std::cout << "created Chunk " << chunkPos << std::endl;
}

Chunk::~Chunk() {
  // std::cout << "deleted Chunk " << chunkPos << std::endl;
}

std::weak_ptr<Chunk> Chunk::getNeighbor(glm::ivec3 off) {
  static const ivec3 mask(9, 3, 1);
  auto greater = equal(off, ivec3(1));
  auto lesser = equal(off, ivec3(-1));

  auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
  int index = tmp.x + tmp.y + tmp.z - 1;
  return neighbors[index];
}

Block* Chunk::getBlockAccrossChunks(ivec3 pos) {
  static const ivec3 mask(9, 3, 1);
  auto greater = greaterThanEqual(pos, size);
  auto lesser = lessThan(pos, ivec3(0));

  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
    int index = tmp.x + tmp.y + tmp.z - 1;

    if(auto neigh = neighbors[index].lock()) {
      ivec3 otherChunkPos = pos - size * (ivec3(greater) - ivec3(lesser));
      return neigh->getBlock(otherChunkPos);
    }
    else return nullptr;
  }
  return (*this)[pos].get();
}

void Chunk::setBlockAccrossChunks(ivec3 pos, Block::unique_ptr_t block) {
  static const ivec3 mask(9, 3, 1);
  auto greater = greaterThanEqual(pos, size);
  auto lesser = lessThan(pos, ivec3(0));

  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
    int index = tmp.x + tmp.y + tmp.z - 1;

    if(auto neigh = neighbors[index].lock()) {
      ivec3 otherChunkPos = pos - size * (ivec3(greater) - ivec3(lesser));
      return neigh->setBlock(otherChunkPos, std::move(block));
    }
  }
  else setBlock(pos, std::move(block));
}

void Chunk::compute() {
  std::lock_guard<std::mutex> lck(computeMutex);
  // indices scheme // TODO: remove from MeshData ?
  solidData.scheme = { 0, 1, 2, 0, 2, 3 };
  transparentData.scheme = { 0, 1, 2, 0, 2, 3 };

  // now generate all the blocks
  ivec3 pos{};
  for(pos.x = 0; pos.x < size.x; pos.x++) {
    for(pos.y = 0; pos.y < size.y; pos.y++) {
      for(pos.z = 0; pos.z < size.z; pos.z++) {

        Block* block = getBlock(pos);
        if(!block->isVisible()) continue;

        // get the block neighbors
        std::array<Block*, 26> neighbors;
        for(size_t i = 0; i < 26; i++) {
          ivec3 npos = pos + neighborOffsets[i];
          auto neigh = getBlockAccrossChunks(npos);
          if(!neigh) continue; // may happen that a neighbor is unloaded
          neighbors[i] = neigh;
        }

        // generate this block's geometry
        if(block->isTransparent()) {
          block->getGeometry()->generateMesh(pos, block, neighbors, transparentData);
        }
        else {
          block->getGeometry()->generateMesh(pos, block, neighbors, solidData);
        }

      }
    }
  }

  computed = true;
  loaded = true;
  mustRecompute = false;
}

void Chunk::markToRecompute() {
  mustRecompute = true;
}

void Chunk::update() {
  if(computeMutex.try_lock()) {

    if(loaded && mustRecompute) {
      computeMutex.unlock();
      compute();
      return update();
    }

    if(computed) {
      computed = false;
      auto model = translate(mat4(1.0), vec3(size * chunkPos));
      solidMesh = std::unique_ptr<Mesh>(nullptr);
      if(solidData.indices.size() != 0) {
        solidMesh = std::make_unique<Mesh>(solidData);
          solidMesh->model = model;
        solidData = {};
      }

      transparentMesh = std::unique_ptr<Mesh>(nullptr);
      if(transparentData.indices.size() != 0) {
        transparentMesh = std::make_unique<Mesh>(transparentData);
          transparentMesh->model = model;
        transparentData = {};
      }
    }

    computeMutex.unlock();
  }
}

void Chunk::drawSolid() {
  if(solidMesh) {
    glBindVertexArray(solidMesh->getVAO());
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(solidMesh->model));
    glDrawElements(GL_TRIANGLES, solidMesh->getVertexCount(), GL_UNSIGNED_INT, nullptr);
  }
}

void Chunk::drawTransparent() {
  if(transparentMesh) {
    glBindVertexArray(transparentMesh->getVAO());
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(transparentMesh->model));
    glDrawElements(GL_TRIANGLES, transparentMesh->getVertexCount(), GL_UNSIGNED_INT, nullptr);
  }
}

bool Chunk::isLoaded() {
  std::lock_guard<std::mutex> lck(computeMutex);
  return loaded;
}

Block* Chunk::getBlock(ivec3 pos) {
  return at(pos).get();
}

void Chunk::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  at(pos) = std::move(block);
  markToRecompute();

  static const ivec3 mask(9, 3, 1);

  auto greater = equal(pos, size - 1);
  auto lesser = equal(pos, ivec3(0));

  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;

    std::vector<int> updates;
    for(int i = 0; i < 2*2*2; i++) {
      int index = tmp.x * ((i&0b100)>>2) + tmp.y * ((i&0b010)>>1) + tmp.z * (i&0b001);
      if(index == 0) continue;
      if(std::find(updates.begin(), updates.end(), index) != updates.end()) continue;
      updates.push_back(index);
      if(auto neigh = neighbors[index - 1].lock()) {
        neigh->markToRecompute();
      }
    }
  }
}
