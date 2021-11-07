#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Chunk.hpp"
#include "gl/Shader.hpp"

using namespace glm;
using std::move;

#ifdef MONCRAFT_CLIENT
AbstractChunk* AbstractChunk::create(ivec3 chunkPos, int chunkSize) {
  return new Chunk(chunkPos, chunkSize);
}
#endif

Chunk::Chunk(ivec3 chunkPos, int chunkSize)
  : AbstractChunk(chunkPos, chunkSize),
    mesh(nullptr),
    loaded(false), computed(false), mustRecompute(false), model(1.f)
{}

Chunk::~Chunk() {
  SafeMesh(std::move(mesh));
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

        // generate this block's model
        block->getModel()->generateMesh(pos, block, neighbors, meshData);
      }
    }
  }

  computed = true;
  loaded = true;
  mustRecompute = false;

  solidOffset = (unsigned int)meshData.indicesSolid.size();
  transpOffset.x = solidOffset + (unsigned int)meshData.indicesTranspX.size();
  transpOffset.y = transpOffset.x + (unsigned int)meshData.indicesTranspY.size();
  transpOffset.z  = transpOffset.y + (unsigned int)meshData.indicesTranspZ.size();

  model = translate(mat4(1.0), vec3(size * chunkPos));
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
      if(hasData()) {
        mesh = std::make_unique<Mesh>(meshData);
        mesh->model = model;
        meshData = {};
      }
      else {
        mesh = std::unique_ptr<Mesh>(nullptr);
      }
    }

    computeMutex.unlock();
  }
}

bool Chunk::hasSolidData() const {
  return solidOffset != 0;
}

bool Chunk::hasTransparentData() const {
  return solidOffset != transpOffset.z;
}

bool Chunk::hasData() const {
  return transpOffset.z != 0;
}

void Chunk::drawSolid() {
  if(mesh && hasSolidData()) {
    glBindVertexArray(mesh->getVAO());
    glUniformMatrix4fv(Shader::getActive()->getUniform(MATRIX_MODEL), 1, GL_FALSE, glm::value_ptr(mesh->model));
    glDrawElements(GL_TRIANGLES, solidOffset, GL_UNSIGNED_INT, nullptr);
  }
}

void Chunk::drawTransparent(vec3 dir) {
  if(mesh && hasTransparentData()) {
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

    const auto modelloc = Shader::getActive()->getUniform(MATRIX_MODEL);
    glBindVertexArray(mesh->getVAO());
    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(mesh->model));

    for(int i = 0; i < 3; i++) {

      if(order[i] == 0 && transpOffset.x != solidOffset) {
        auto offset = solidOffset;
        if(dir.x > 0) offset += signOffset;
        glDrawElements(GL_TRIANGLES, transpOffset.x - solidOffset, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(GLuint)));
      }
      if(order[i] == 1 && transpOffset.y != transpOffset.x) {
        auto offset = transpOffset.x;
        if(dir.y > 0) offset += signOffset;
        glDrawElements(GL_TRIANGLES, transpOffset.y - transpOffset.x, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(GLuint)));
      }
      if(order[i] == 2 && transpOffset.z != transpOffset.y) {
        auto offset = transpOffset.y;
        if(dir.z > 0) offset += signOffset;
        glDrawElements(GL_TRIANGLES, transpOffset.z - transpOffset.y, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(GLuint)));
      }
    }
  }
}

void Chunk::drawAllAsSolid() {
  if(mesh && hasData()) {
    glBindVertexArray(mesh->getVAO());
    glUniformMatrix4fv(Shader::getActive()->getUniform(MATRIX_MODEL), 1, GL_FALSE, glm::value_ptr(mesh->model));
    glDrawElements(GL_TRIANGLES, transpOffset.z, GL_UNSIGNED_INT, nullptr);
  }
}

bool Chunk::isComputed() {
  std::lock_guard<std::mutex> lck(computeMutex);
  return loaded;
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
      if(auto absNeigh = neighbors[(size_t)index - 1].lock()) {
        if(auto neigh = std::dynamic_pointer_cast<Chunk>(absNeigh)) {
          neigh->markToRecompute();
        }
      }
    }
  }
}
