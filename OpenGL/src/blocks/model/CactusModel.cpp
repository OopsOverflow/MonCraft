#include "CactusModel.hpp"

#include <glm/glm.hpp>
#include <stddef.h>
#include <algorithm>
#include <utility>
#include <vector>

#include "blocks/Block.hpp"
#include "blocks/model/DefaultModel.hpp"
#include "gl/Mesh.hpp"

using namespace glm;

CactusModel::CactusModel() {}

CactusModel* CactusModel::get() {
  static CactusModel inst;
  return &inst;
}

void CactusModel::genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
    auto& _ind = data.indicesSolid;
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

void CactusModel::generateMesh(ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  for(auto const& off : blockFaceOffsets) {
    genFace(pos, off.second, block, neighbors, data);
  }
}

/// below is all the data and lookup tables

static const float off = 1.f / 16.f;

const BlockData<3> CactusModel::blockPositions = {
  FaceData<3>{ // TOP
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
     0.5f,  0.5f,  0.5f - off,
    -0.5f,  0.5f,  0.5f - off,
    -0.5,  -0.5f,  0.5f - off,
     0.5f, -0.5f,  0.5f - off,
  }, { // RIGHT
     0.5f - off,  0.5f, -0.5f,
     0.5f - off,  0.5f,  0.5f,
     0.5f - off, -0.5f,  0.5f,
     0.5f - off, -0.5f, -0.5f,
  }, { // BACK
    -0.5f,  0.5f, -0.5f + off,
     0.5f,  0.5f, -0.5f + off,
     0.5f, -0.5f, -0.5f + off,
    -0.5f, -0.5f, -0.5f + off,
  }, { // LEFT
    -0.5f + off,  0.5f,  0.5f,
    -0.5f + off,  0.5f, -0.5f,
    -0.5f + off, -0.5f, -0.5f,
    -0.5f + off, -0.5f,  0.5f,
  }
};
