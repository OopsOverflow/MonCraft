#include <algorithm>

#include "DefaultGeometry.hpp"
#include "blocks/Block.hpp"

using namespace glm;

DefaultBlockGeometry::DefaultBlockGeometry() {}

DefaultBlockGeometry* DefaultBlockGeometry::get() {
  static DefaultBlockGeometry inst;
  return &inst;
}

std::array<GLfloat, 4> DefaultBlockGeometry::genOcclusion(glm::ivec3 pos, std::array<Block*, 26> const& neighbors, BlockFace face) const {
  std::array<GLfloat, 4> occl{};

  auto const& offsets = blockOcclusionOffsets[static_cast<size_t>(face)];
  std::array<bool, 8> b{};

  for(int i = 0; i < 8; i++) {
    Block* neigh = neighbors[offsets[i]];
    b[i] = neigh->isSolid() && !neigh->isTransparent();
  }

  occl[0] = (float)(b[0] + b[1] + b[2]);
  occl[1] = (float)(b[2] + b[3] + b[4]);
  occl[2] = (float)(b[4] + b[5] + b[6]);
  occl[3] = (float)(b[6] + b[7] + b[0]);

  return occl;
}

void DefaultBlockGeometry::genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
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
  auto uvFace = genFaceUV(indexUV);
  _uvs.insert(_uvs.end(), uvFace.begin(), uvFace.end());

  // occlusion
  auto occl = genOcclusion(pos, neighbors, face);
  _occl.insert(_occl.end(), occl.begin(), occl.end());

  // normalMapCoords
  _normm.insert(_normm.end(), faceNormalMap.begin(), faceNormalMap.end());

}

void DefaultBlockGeometry::generateMesh(ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  for(auto const& off : blockFaceOffsets) {
    auto neigh = neighbors[off.first];
    if(!neigh->isOpaque() || (neigh->isTransparent() && !block->isTransparent())) {
      genFace(pos, off.second, block, neighbors, data);
    }
  }
}

/// below is all the data and lookup tables

const BlockData<3> DefaultBlockGeometry::blockPositions = {
  face_t<3>{ // TOP
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
  }, { // BOTTOM
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
  }, { // FRONT
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
  }, { // RIGHT
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
  }, { // BACK
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
  }, { // LEFT
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
  }
};

const BlockData<3> DefaultBlockGeometry::blockNormals {
  face_t<3>{ // TOP
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  }, { // BOTTOM
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
  }, { // FRONT
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
  }, { // RIGHT
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
  }, { // BACK
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
  }, { // LEFT
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
  }
};

// tells which neighbors to look at when computing a block occlusion
// the ints stored are indices to a neighbor in a neighbor array (see Chunk.hpp neighbors)
const std::array<std::array<int, 8>, 6> DefaultBlockGeometry::blockOcclusionOffsets = {
  std::array<int, 8> { // TOP
    20, 21, 3, 12, 11, 13, 4, 22,
  },
  std::array<int, 8> { // BOTTOM
    14, 15, 6, 24, 23, 25, 7, 16,
  },
  std::array<int, 8> { // FRONT
    9, 12, 3, 21, 18, 24, 6, 15,
  },
  std::array<int, 8> { // RIGHT
    10, 13, 11, 12, 9, 15, 14, 16,
  },
  std::array<int, 8> { // BACK
    19, 22, 4, 13, 10, 16, 7, 25,
  },
  std::array<int, 8> { // LEFT
    18, 21, 20, 22, 19, 25, 23, 24,
  },
};

// tells which neighbor corresponds to which BlockFace
// the ints stored are indices to a neighbor in a neighbor array (see Chunk.hpp neighbors)
const std::array<std::pair<int, BlockFace>, 6> DefaultBlockGeometry::blockFaceOffsets = {
  std::pair<int, BlockFace>
  {17, BlockFace::LEFT},
  {8, BlockFace::RIGHT},
  {5, BlockFace::BOTTOM},
  {2, BlockFace::TOP},
  {1, BlockFace::BACK},
  {0, BlockFace::FRONT},
};

const face_t<2> DefaultBlockGeometry::faceNormalMap = {
  1.f, 0.f,
  0.5f, 0.f,
  0.5f, 1.f,
  1.f, 1.f,
};
