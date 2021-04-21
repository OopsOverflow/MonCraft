#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Chunk.hpp"
#include "BlockGeom.hpp"

using glm::ivec2;
using glm::vec3;
using glm::ivec3;
using std::move;

#include "Debug.hpp"

Chunk::Chunk(ivec3 chunkPos, Blocks blocks)
  : chunkPos(chunkPos), blocks(new Blocks(move(blocks))), mesh(nullptr)
{
  generateMesh();
  // std::cout << "created chunk " << chunkPos << std::endl;
}

Chunk::~Chunk() {
  // std::cout << "deleted chunk " << chunkPos << std::endl;
  delete mesh;
  delete blocks;
}

// /!\ extra care must be taken here.
//  - uses operator[] which is unsafe (no bounds checks)
//  - assumes chunk is loaded
bool Chunk::isSolid(ivec3 pos) {
  return (*blocks)[pos]->type != BlockType::Air;
}

// TODO: move out of here (util ?)
face_t<2> getFaceUV(glm::ivec2 index) {
  static const float atlasSize = 8.f;
  return face_t<2> {
    (index.x + 1) / atlasSize, (index.y + 0) / atlasSize,
    (index.x + 0) / atlasSize, (index.y + 0) / atlasSize,
    (index.x + 0) / atlasSize, (index.y + 1) / atlasSize,
    (index.x + 1) / atlasSize, (index.y + 1) / atlasSize,
  };
}

std::array<GLfloat, 4> Chunk::genOcclusion(ivec3 pos, BlockFace face) {
  std::array<GLfloat, 4> occl = { 0.f, 0.f, 0.f, 0.f };

  auto const& offsets = blockOcclusionOffsets[static_cast<size_t>(face)];
  std::array<bool, 8> b;

  for(int i = 0; i < 8; i++) {
    b[i] = isSolid(pos + offsets[i]);
  }

  occl[0] = b[0] + b[1] + b[2];
  occl[1] = b[2] + b[3] + b[4];
  occl[2] = b[4] + b[5] + b[6];
  occl[3] = b[6] + b[7] + b[0];

  return occl;
}

void Chunk::generateMesh() {
  // indices scheme
  std::vector<int> scheme = { 0, 1, 2, 0, 2, 3 };

  auto genFace = [&](ivec3 pos, BlockFace face) {
    // indices
    indices.insert(indices.end(), scheme.begin(), scheme.end());
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), [](int x) { return x+4; });

    // positions
    auto posFace = blockPositions[(size_t)face];
    positions.insert(positions.end(), posFace.begin(), posFace.end());
    for(int i = 0, k = 0; i < 12; i++, k = (k+1) % 3) {
      positions[positions.size() - 12 + i] += pos[k] - 1;
    }

    // normals
    auto normFace = blockNormals[(size_t)face];
    normals.insert(normals.end(), normFace.begin(), normFace.end());

    // textureCoords
    auto indexUV = getBlock(pos)->getFaceUVs(face);
    auto uvFace = getFaceUV(indexUV);
    textureCoords.insert(textureCoords.end(), uvFace.begin(), uvFace.end());

    // occlusion
    auto occl = genOcclusion(pos, face);
    occlusion.insert(occlusion.end(), occl.begin(), occl.end());
  };

  ivec3 pos;
  for(pos.x = 1; pos.x < blocks->size.x-1; pos.x++) {
    for(pos.y = 1; pos.y < blocks->size.y-1; pos.y++) {
      for(pos.z = 1; pos.z < blocks->size.z-1; pos.z++) {
        if(!isSolid(pos)) continue;
        if(!isSolid(pos + ivec3(1, 0, 0)))  genFace(pos, BlockFace::RIGHT);
        if(!isSolid(pos + ivec3(-1, 0, 0))) genFace(pos, BlockFace::LEFT);
        if(!isSolid(pos + ivec3(0, 1, 0)))  genFace(pos, BlockFace::TOP);
        if(!isSolid(pos + ivec3(0, -1, 0))) genFace(pos, BlockFace::BOTTOM);
        if(!isSolid(pos + ivec3(0, 0, 1)))  genFace(pos, BlockFace::FRONT);
        if(!isSolid(pos + ivec3(0, 0, -1))) genFace(pos, BlockFace::BACK);
      }
    }
  }
}

Mesh const& Chunk::getMesh() {
  if(mesh == nullptr) {
    mesh = new Mesh(positions, normals, textureCoords, occlusion, indices);
    mesh->model = glm::translate(mesh->model, vec3(chunkPos));
  }
  return *mesh;
}

Block* Chunk::getBlock(ivec3 pos) {
  if(!blocks) return nullptr;
  return blocks->at(pos).get();
}

void Chunk::unload() {
  delete blocks;
  blocks = nullptr;
}
