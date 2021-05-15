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
    mesh(nullptr),
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
  meshData.scheme = { 0, 1, 2, 0, 2, 3 };

  DataStore<Block*, 3> blocksCache(size + 2); // a cache to limit calls to getBlockAccrossChunks

  ivec3 pos{};
  for(pos.x = 0; pos.x < size.x + 2; pos.x++) {
    for(pos.y = 0; pos.y < size.y + 2; pos.y++) {
      for(pos.z = 0; pos.z < size.z + 2; pos.z++) {
        blocksCache[pos] = getBlockAccrossChunks(pos - 1);
      }
    }
  }

  std::array<Block*, 26> neighbors;

  // now generate all the blocks
  for(pos.x = 0; pos.x < size.x; pos.x++) {
    for(pos.y = 0; pos.y < size.y; pos.y++) {
      for(pos.z = 0; pos.z < size.z; pos.z++) {

        Block* block = blocksCache[pos + 1];
        if(!block->isVisible()) continue;

        // get the block neighbors
        bool neighborsValid = true;
        for(size_t i = 0; i < 26; i++) {
          ivec3 npos = pos + neighborOffsets[i];
          auto neigh = blocksCache[npos + 1];
          if(!neigh) neighborsValid = false; // may happen that a neighbor is unloaded
          neighbors[i] = neigh;
        }

        if(!neighborsValid) continue;

        // generate this block's geometry
        block->getGeometry()->generateMesh(pos, block, neighbors, meshData);
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
      mesh = std::unique_ptr<Mesh>(nullptr);
      solidOffset = meshData.indicesSolid.size();
      transpOffset.x = solidOffset + meshData.indicesTranspX.size();
      transpOffset.y = transpOffset.x + meshData.indicesTranspY.size();
      transpOffset.z  = transpOffset.y + meshData.indicesTranspZ.size();
      if(transpOffset.z != 0) {
        mesh = std::make_unique<Mesh>(meshData);
        mesh->model = model;
        meshData = {};
      }
    }

    computeMutex.unlock();
  }
}

void Chunk::drawSolid() {
  if(mesh && solidOffset != 0) {
    glBindVertexArray(mesh->getVAO());
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh->model));
    glDrawElements(GL_TRIANGLES, solidOffset, GL_UNSIGNED_INT, nullptr);
  }
}

void Chunk::drawTransparent(vec3 dir) {
  if(mesh) {
    auto signOffset = transpOffset.z - solidOffset;

    // find in which order of x/y/z components to draw
    // COMBAK: this is not perfect because the geometry is not generated in
    // exact order (back to front).
    vec3 absDir = abs(dir);
    ivec3 order;
    if(absDir.x > absDir.y)
      if(absDir.x > absDir.z)
        if(absDir.y > absDir.z)
          order = ivec3(0, 1, 2);
        else
          order = ivec3(0, 2, 1);
      else
        order = ivec3(2, 0, 1);
    else
      if(absDir.y > absDir.z)
        if(absDir.x > absDir.z)
          order = ivec3(1, 0, 2);
        else
          order = ivec3(1, 2, 0);
      else
        order = ivec3(2, 1, 0);
    // order = 2 - order;

    for(int i = 0; i < 3; i++) {

      if(order[i] == 0 && transpOffset.x != solidOffset) {
        glBindVertexArray(mesh->getVAO());
        glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh->model));
        auto offset = solidOffset;
        if(dir.x > 0) offset += signOffset;
        glDrawElements(GL_TRIANGLES, transpOffset.x - solidOffset, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(GLuint)));
      }
      if(order[i] == 1 && transpOffset.y != transpOffset.x) {
        glBindVertexArray(mesh->getVAO());
        glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh->model));
        auto offset = transpOffset.x;
        if(dir.y > 0) offset += signOffset;
        glDrawElements(GL_TRIANGLES, transpOffset.y - transpOffset.x, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(GLuint)));
      }
      if(order[i] == 2 && transpOffset.z != transpOffset.y) {
        glBindVertexArray(mesh->getVAO());
        glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh->model));
        auto offset = transpOffset.y;
        if(dir.z > 0) offset += signOffset;
        glDrawElements(GL_TRIANGLES, transpOffset.z - transpOffset.y, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(GLuint)));
      }

    }


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
