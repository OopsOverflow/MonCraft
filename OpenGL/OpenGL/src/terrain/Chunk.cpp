#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Chunk.hpp"
#include "BlockGeom.hpp"

using glm::ivec2;
using glm::vec3;
using glm::ivec3;
using std::move;

Chunk::Chunk(ivec2 chunkPos, Blocks blocks)
  : chunkPos(chunkPos), blocks(move(blocks)), mesh(nullptr)
{
  generateMesh();
    // std::cout << "created chunk (" << chunkPos.x << ", " << chunkPos.y << ")" << std::endl;
}

Chunk::~Chunk() {
  // std::cout << "deleted chunk (" << chunkPos.x << ", " << chunkPos.y << ")" << std::endl;
  delete mesh;
}

bool Chunk::isSolid(ivec3 pos) {
  return blocks.at(pos)->type != BlockType::Air;
}

face_t<2> getFaceUV(glm::ivec2 index) {
  return face_t<2> {
    (index.x + 1) / 6.f, (index.y + 0) / 2.f,
    (index.x + 0) / 6.f, (index.y + 0) / 2.f,
    (index.x + 0) / 6.f, (index.y + 1) / 2.f,
    (index.x + 1) / 6.f, (index.y + 1) / 2.f,
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
  auto begin = scheme.begin();
  auto end = scheme.end();

  auto genFace = [&](ivec3 pos, BlockFace face) {
    // indices
    std::copy(begin, end, std::back_inserter(indices));
    std::transform(begin, end, begin,[](int x) { return x+4; });

    // positions
    auto posFace = blockPositions[static_cast<size_t>(face)];
    std::copy(posFace.begin(), posFace.end(), std::back_inserter(positions));
    for(int i = 0, k = 0; i < 12; i++, k = (k+1) % 3) {
      positions[positions.size()- 12 + i] += pos[k] - 1;
    }

    // normals
    auto normFace = blockNormals[static_cast<size_t>(face)];
    std::copy(normFace.begin(), normFace.end(), std::back_inserter(normals));

    // textureCoords
    auto indexUV = getBlock(pos)->getFaceUVs(face);
    auto uvFace = getFaceUV(indexUV);
    std::copy(uvFace.begin(), uvFace.end(), std::back_inserter(textureCoords));

    // occlusion
    auto occl = genOcclusion(pos, face);
    std::copy(occl.begin(), occl.end(), std::back_inserter(occlusion));
  };

  ivec3 pos;
  for(pos.x = 1; pos.x < blocks.size.x-1; pos.x++) {
    for(pos.y = 1; pos.y < blocks.size.y-1; pos.y++) {
      for(pos.z = 1; pos.z < blocks.size.z-1; pos.z++) {
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
    mesh->model = glm::translate(mesh->model, vec3(chunkPos.x, 0, chunkPos.y));
  }
  return *mesh;
}

ivec2 Chunk::getPosition() const {
  return chunkPos;
}


Block* Chunk::getBlock(ivec3 pos) {
  return blocks.at(pos).get();
}
