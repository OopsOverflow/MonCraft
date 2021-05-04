#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Chunk.hpp"
#include "BlockGeom.hpp"
#include "gl/Shader.hpp"

using namespace glm;
using std::move;

#include "debug/Debug.hpp"

Chunk::Chunk(int size)
  : DataStore<Block::unique_ptr_t, 3>(glm::ivec3(size))
{
}

ChunkMesh::ChunkMesh(ivec3 chunkPos, std::shared_ptr<Chunk> chunk)
  : chunk(chunk),
    chunkPos(chunkPos),
    mesh(nullptr)
{
  generateMeshData();
  // std::cout << "created ChunkMesh " << chunkPos << std::endl;
}

ChunkMesh::~ChunkMesh() {
  // std::cout << "deleted ChunkMesh " << chunkPos << std::endl;
  delete mesh;
}

// /!\ extra care must be taken here.
//  - uses operator[] which is unsafe (no bounds checks)
//  - assumes ChunkMesh is loaded
bool ChunkMesh::isSolid(ivec3 pos) {
  if(any(lessThan(pos, ivec3(0))) || any(greaterThanEqual(pos, chunk->size))) {
    return false; // TODO
  }
  return (*chunk)[pos]->type != BlockType::Air;
}

// TODO: move out of here (util ?)
face_t<2> getFaceUV(ivec2 index) {
  static const float atlasSize = 8.f;
  return face_t<2> {
    (index.x + 1) / atlasSize, (index.y + 0) / atlasSize,
    (index.x + 0) / atlasSize, (index.y + 0) / atlasSize,
    (index.x + 0) / atlasSize, (index.y + 1) / atlasSize,
    (index.x + 1) / atlasSize, (index.y + 1) / atlasSize,
  };
}

std::array<GLfloat, 4> ChunkMesh::genOcclusion(ivec3 pos, BlockFace face) {
  std::array<GLfloat, 4> occl = { 0.f, 0.f, 0.f, 0.f };

  auto const& offsets = blockOcclusionOffsets[static_cast<size_t>(face)];
  std::array<bool, 8> b{};

  for(int i = 0; i < 8; i++) {
    b[i] = isSolid(pos + offsets[i]);
  }

  occl[0] = b[0] + b[1] + b[2];
  occl[1] = b[2] + b[3] + b[4];
  occl[2] = b[4] + b[5] + b[6];
  occl[3] = b[6] + b[7] + b[0];

  return occl;
}

void ChunkMesh::generateMeshData() {
  // indices scheme
  std::vector<int> scheme = { 0, 1, 2, 0, 2, 3 };

  auto genFace = [&](ivec3 pos, BlockFace face) {
    // indices
    indices.insert(indices.end(), scheme.begin(), scheme.end());
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), [](int x) { return x+4; });

    // positions
    auto& posFace = blockPositions[(size_t)face];
    positions.insert(positions.end(), posFace.begin(), posFace.end());
    for(int i = 0, k = 0; i < 12; i++, k = (k+1) % 3) {
      positions[positions.size() - 12 + i] += pos[k] - 1;
    }

    // normals
    auto& normFace = blockNormals[(size_t)face];
    normals.insert(normals.end(), normFace.begin(), normFace.end());

    // textureCoords
    auto indexUV = getBlock(pos)->getFaceUVs(face);
    auto uvFace = getFaceUV(indexUV);
    textureCoords.insert(textureCoords.end(), uvFace.begin(), uvFace.end());

    // occlusion
    auto occl = genOcclusion(pos, face);
    occlusion.insert(occlusion.end(), occl.begin(), occl.end());
  };

  ivec3 pos{};
  for(pos.x = 0; pos.x < chunk->size.x; pos.x++) {
    for(pos.y = 0; pos.y < chunk->size.y; pos.y++) {
      for(pos.z = 0; pos.z < chunk->size.z; pos.z++) {
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

Mesh const& ChunkMesh::getMesh() {
  if(mesh == nullptr) {
    mesh = new Mesh(positions, normals, textureCoords, occlusion, indices);
    mesh->model = translate(mesh->model, vec3(chunkPos));

    positions = {};
    normals = {};
    textureCoords = {};
    occlusion = {};
    indices = {};
  }
  return *mesh;
}

void ChunkMesh::updateMesh() {
  generateMeshData();
  if(mesh) {
    delete mesh;
    getMesh(); // this will re-generate the mesh
  }
}

Block* ChunkMesh::getBlock(ivec3 pos) {
  return chunk->at(pos).get();
}

void ChunkMesh::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  chunk->at(pos) = std::move(block);
  updateMesh();
}
