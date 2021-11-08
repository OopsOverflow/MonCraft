#include "DefaultModel.hpp"

#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "blocks/Block.hpp"
#include "blocks/model/BlockModel.hpp"
#include "gl/Mesh.hpp"

using namespace glm;

DefaultBlockModel::DefaultBlockModel()
{}

DefaultBlockModel* DefaultBlockModel::get() {
  static DefaultBlockModel inst;
  return &inst;
}

face_t<1> DefaultBlockModel::genOcclusion(glm::ivec3 pos, std::array<Block*, 26> const& neighbors, BlockFace face) const {
  std::array<GLfloat, 4> occl{};

  std::array<bool, 9> b{};

  auto const& offsets = blockOcclusionOffsets[static_cast<size_t>(face)];

  for(int i = 0; i < 9; i++) {
    Block* neigh = neighbors[offsets[i]];
    b[i] = neigh->isSolid() && !neigh->isTransparent();
  }

  occl[0] = min((float)(b[0] + b[1] + b[2] + b[8]), 3.f);
  occl[1] = min((float)(b[2] + b[3] + b[4] + b[8]), 3.f);
  occl[2] = min((float)(b[4] + b[5] + b[6] + b[8]), 3.f);
  occl[3] = min((float)(b[6] + b[7] + b[0] + b[8]), 3.f);

  return occl;
}

void DefaultBlockModel::genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  auto& _ind =
    !block->isTransparent() ? data.indicesSolid :
    face == BlockFace::LEFT || face == BlockFace::RIGHT ? data.indicesTranspX :
    face == BlockFace::TOP || face == BlockFace::BOTTOM ? data.indicesTranspY :
    data.indicesTranspZ;
  auto& _scheme = data.scheme;
  auto& _pos  = data.positions;
  auto& _norm = data.normals;
  auto& _uvs  = data.textureCoords;
  auto& _occl = data.occlusion;
  auto& _normm = data.normalMapCoords;

  // indices
  _ind.insert(_ind.end(), _scheme.begin(), _scheme.end());
  std::transform(_scheme.begin(), _scheme.end(), _scheme.begin(), [](int x) { return x+4; });

  // positions
  auto& posFace = blockPositions[(size_t)face];
  _pos.insert(_pos.end(), posFace.begin(), posFace.end());
  for(int i = 0, k = 0; i < 12; i++, k = (k+1) % 3) {
    _pos[_pos.size() - 12 + i] += pos[k];
  }

  // normals
  auto const& normFace = blockNormals[(size_t)face];
  _norm.insert(_norm.end(), normFace.begin(), normFace.end());

  // textureCoords
  auto indexUV = block->getFaceUVs(face);
  auto uvFace = computeUV(indexUV, faceUVs);
  _uvs.insert(_uvs.end(), uvFace.begin(), uvFace.end());

  // occlusion
  auto occl = genOcclusion(pos, neighbors, face);
  _occl.insert(_occl.end(), occl.begin(), occl.end());

  // normalMapCoords
  _normm.insert(_normm.end(), faceNormalMap.begin(), faceNormalMap.end());

}

void DefaultBlockModel::generateMesh(ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  for(auto const& off : blockFaceOffsets) {
    auto neigh = neighbors[off.first];

    if(neigh->isOpaque())
      continue;

    else if(block->isTransparent() && block->type == neigh->type)
      continue;

    else
      genFace(pos, off.second, block, neighbors, data);
  }
}

/// below is all the data and lookup tables

const Quad<2> DefaultBlockModel::faceUVs = { vec2
  { 1.f, 0.f },
  { 0.f, 0.f },
  { 0.f, 1.f },
  { 1.f, 1.f },
};

const QuadMesh<3> DefaultBlockModel::mesh = {
  std::vector<Quad<3>> { // TOP
    { glm::vec3
      { -0.5f,  0.5f,  0.5f },
      {  0.5f,  0.5f,  0.5f },
      {  0.5f,  0.5f, -0.5f },
      { -0.5f,  0.5f, -0.5f },
    }
  }, { // BOTTOM
    { glm::vec3
      {  0.5f, -0.5f,  0.5f },
      { -0.5f, -0.5f,  0.5f },
      { -0.5f, -0.5f, -0.5f },
      {  0.5f, -0.5f, -0.5f },
    }
  }, { // FRONT
    { glm::vec3
      {  0.5f,  0.5f,  0.5f },
      { -0.5f,  0.5f,  0.5f },
      { -0.5f, -0.5f,  0.5f },
      {  0.5f, -0.5f,  0.5f },
    }
  }, { // RIGHT
    { glm::vec3
      { 0.5f,  0.5f, -0.5f },
      { 0.5f,  0.5f,  0.5f },
      { 0.5f, -0.5f,  0.5f },
      { 0.5f, -0.5f, -0.5f },
    }
  }, { // BACK
    { glm::vec3
      { -0.5f,  0.5f, -0.5f },
      {  0.5f,  0.5f, -0.5f },
      {  0.5f, -0.5f, -0.5f },
      { -0.5f, -0.5f, -0.5f },
    }
  }, { // LEFT
    { glm::vec3
      { -0.5f,  0.5f,  0.5f },
      { -0.5f,  0.5f, -0.5f },
      { -0.5f, -0.5f, -0.5f },
      { -0.5f, -0.5f,  0.5f },
    }
  }, { // INNER

  }
};

const QuadMesh<3> DefaultBlockModel::meshNormals {
  std::vector<Quad<3>>{ // TOP
    { glm::vec3
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
    }
  }, { // BOTTOM
    { glm::vec3
      { 0.0f, -1.0f, 0.0f },
      { 0.0f, -1.0f, 0.0f },
      { 0.0f, -1.0f, 0.0f },
      { 0.0f, -1.0f, 0.0f },
    }
  }, { // FRONT
    { glm::vec3
      { 0.0f, 0.0f, 1.0f },
      { 0.0f, 0.0f, 1.0f },
      { 0.0f, 0.0f, 1.0f },
      { 0.0f, 0.0f, 1.0f },
    }
  }, { // RIGHT
    { glm::vec3
      { 1.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 0.0f },
    }
  }, { // BACK
    { glm::vec3
      { 0.0f, 0.0f, -1.0f },
      { 0.0f, 0.0f, -1.0f },
      { 0.0f, 0.0f, -1.0f },
      { 0.0f, 0.0f, -1.0f },
    }
  }, { // LEFT
    { glm::vec3
      { -1.0f, 0.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f },
    }
  }, { // INNER

  }
};

// tells which neighbors to look at when computing a block occlusion
// the ints stored are indices to a neighbor in a neighbor array (see Chunk.hpp neighbors)
const std::array<std::array<int, 9>, 6> DefaultBlockModel::blockOcclusionOffsets = {
  std::array<int, 9> { // TOP
    20, 21, 3, 12, 11, 13, 4, 22, 2,
  }, { // BOTTOM
    14, 15, 6, 24, 23, 25, 7, 16, 5,
  }, { // FRONT
    9, 12, 3, 21, 18, 24, 6, 15, 0,
  }, { // RIGHT
    10, 13, 11, 12, 9, 15, 14, 16, 8,
  }, { // BACK
    19, 22, 4, 13, 10, 16, 7, 25, 1,
  }, { // LEFT
    18, 21, 20, 22, 19, 25, 23, 24, 17,
  },
};

// tells which neighbor corresponds to which BlockFace
// the ints stored are indices to a neighbor in a neighbor array (see Chunk.hpp neighbors)
const std::array<std::pair<int, BlockFace>, 6> DefaultBlockModel::blockFaceOffsets = {
  std::pair<int, BlockFace>
  {2, BlockFace::TOP},
  {5, BlockFace::BOTTOM},
  {0, BlockFace::FRONT},
  {8, BlockFace::RIGHT},
  {1, BlockFace::BACK},
  {17, BlockFace::LEFT},
};

const face_t<2> DefaultBlockModel::faceNormalMap = {
  1.f, 0.f,
  0.5f, 0.f,
  0.5f, 1.f,
  1.f, 1.f,
};


const BlockMeshData DefaultBlockModel::blockPositions(flatten(mesh));
const BlockMeshData DefaultBlockModel::blockNormals(flatten(meshNormals));
