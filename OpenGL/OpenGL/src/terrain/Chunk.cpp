#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Chunk.hpp"
#include "BlockGeom.hpp"
#include "gl/Shader.hpp"

using namespace glm;
using std::move;

#include "debug/Debug.hpp"

Chunk::Chunk(ivec3 chunkPos, int chunkSize)
  : DataStore<Block::unique_ptr_t, 3>(glm::ivec3(chunkSize)),
    chunkPos(chunkPos),
    mesh(nullptr),
    computed(false)
{
  // std::cout << "created Chunk " << chunkPos << std::endl;
}

Chunk::~Chunk() {
  // std::cout << "deleted Chunk " << chunkPos << std::endl;
}

// /!\ extra care must be taken here.
//  - uses operator[] which is unsafe (no bounds checks)
bool Chunk::isSolid(ivec3 pos) {
  static const ivec3 mask(9, 3, 1);

  auto greater = greaterThanEqual(pos, size);
  auto lesser = lessThan(pos, ivec3(0));
  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;
    int index = tmp.x + tmp.y + tmp.z - 1;

    if(auto neigh = neighbors[index].lock()) {
      return neigh->isSolidNoChecks(pos - size * (ivec3(greater) - ivec3(lesser)));
    }
    else return false;
  }
  return (*this)[pos]->type != BlockType::Air;
}

// /!\ extra care must be taken here.
//  - uses operator[] which is unsafe (no bounds checks)
bool Chunk::isSolidNoChecks(ivec3 pos) {
  return (*this)[pos]->type != BlockType::Air;
}

void Chunk::compute() {
  std::lock_guard<std::mutex> lck(computeMutex);
  // indices scheme
  std::vector<int> scheme = { 0, 1, 2, 0, 2, 3 };

  DataStore<bool, 3> solidBlocks(size + 2); // a cache to limit calls to isSolid()

  auto genOcclusion = [&solidBlocks](ivec3 pos, BlockFace face) -> std::array<GLfloat, 4> {
    std::array<GLfloat, 4> occl{};

    auto const& offsets = blockOcclusionOffsets[static_cast<size_t>(face)];
    std::array<bool, 8> b{};

    for(int i = 0; i < 8; i++) {
      b[i] = solidBlocks[pos + offsets[i] + 1];
    }

    occl[0] = b[0] + b[1] + b[2];
    occl[1] = b[2] + b[3] + b[4];
    occl[2] = b[4] + b[5] + b[6];
    occl[3] = b[6] + b[7] + b[0];

    return occl;
  };

  auto genFaceUV = [](glm::ivec2 index) {
    static const int atlasSize = 8;

    return face_t<2>{
      (index.x + 1.f) / atlasSize, (index.y + 0.f) / atlasSize,
      (index.x + 0.f) / atlasSize, (index.y + 0.f) / atlasSize,
      (index.x + 0.f) / atlasSize, (index.y + 1.f) / atlasSize,
      (index.x + 1.f) / atlasSize, (index.y + 1.f) / atlasSize,
    };
  };

  auto genFace = [&](ivec3 pos, BlockFace face) {
    // indices
    indices.insert(indices.end(), scheme.begin(), scheme.end());
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), [](int x) { return x+4; });

    // positions
    auto& posFace = blockPositions[(size_t)face];
    positions.insert(positions.end(), posFace.begin(), posFace.end());
    for(int i = 0, k = 0; i < 12; i++, k = (k+1) % 3) {
      positions[positions.size() - 12 + i] += pos[k];
    }

    // normals
    auto& normFace = blockNormals[(size_t)face];
    normals.insert(normals.end(), normFace.begin(), normFace.end());

    // textureCoords
    auto indexUV = getBlock(pos)->getFaceUVs(face);
    auto uvFace = genFaceUV(indexUV);
    textureCoords.insert(textureCoords.end(), uvFace.begin(), uvFace.end());

    // occlusion
    auto occl = genOcclusion(pos, face);
    occlusion.insert(occlusion.end(), occl.begin(), occl.end());
  };

  ivec3 pos{};

  // cache the solid state of blocks
  for(pos.x = 0; pos.x < size.x + 2; pos.x++) {
    for(pos.y = 0; pos.y < size.y + 2; pos.y++) {
      for(pos.z = 0; pos.z < size.z + 2; pos.z++) {
        solidBlocks[pos] = isSolid(pos - 1);
      }
    }
  }

  // finally generate the face quads
  for(pos.x = 0; pos.x < size.x; pos.x++) {
    for(pos.y = 0; pos.y < size.y; pos.y++) {
      for(pos.z = 0; pos.z < size.z; pos.z++) {
        if(!solidBlocks[pos + 1]) continue;
        if(!solidBlocks[pos + ivec3(1, 0, 0) + 1])  genFace(pos, BlockFace::RIGHT);
        if(!solidBlocks[pos + ivec3(-1, 0, 0) + 1]) genFace(pos, BlockFace::LEFT);
        if(!solidBlocks[pos + ivec3(0, 1, 0) + 1])  genFace(pos, BlockFace::TOP);
        if(!solidBlocks[pos + ivec3(0, -1, 0) + 1]) genFace(pos, BlockFace::BOTTOM);
        if(!solidBlocks[pos + ivec3(0, 0, 1) + 1])  genFace(pos, BlockFace::FRONT);
        if(!solidBlocks[pos + ivec3(0, 0, -1) + 1]) genFace(pos, BlockFace::BACK);
      }
    }
  }

  if(indices.size() != 0) { // that chunk is not filled with air
    computed = true;
  }
}

void Chunk::update() {
  if(computeMutex.try_lock()) {
    if(computed) {
      mesh = std::make_unique<Mesh>(positions, normals, textureCoords, occlusion, indices);
      mesh->model = translate(mesh->model, vec3(size * chunkPos));

      positions = {};
      normals = {};
      textureCoords = {};
      occlusion = {};
      indices = {};
      computed = false;
    }
    computeMutex.unlock();
  }
}

void Chunk::draw() {
  update();
  if(mesh) {
    glBindVertexArray(mesh->getVAO());
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh->model));
    glDrawElements(GL_TRIANGLES, mesh->getVertexCount(), GL_UNSIGNED_INT, nullptr);
  }
}

bool Chunk::isLoaded() {
  std::lock_guard<std::mutex> lck(computeMutex);
  return computed || mesh;
}

Block* Chunk::getBlock(ivec3 pos) {
  return at(pos).get();
}

void Chunk::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  at(pos) = std::move(block);
}
