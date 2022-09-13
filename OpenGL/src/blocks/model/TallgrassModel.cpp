#include "TallgrassModel.hpp"

#include <glm/glm.hpp>
#include <stddef.h>
#include <algorithm>
#include <cmath>
#include <vector>

#include "blocks/Block.hpp"
#include "blocks/model/DefaultModel.hpp"
#include "gl/Mesh.hpp"

TallgrassModel* TallgrassModel::get() {
  static TallgrassModel inst;
  return &inst;
}


void TallgrassModel::generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  auto& _ind = data.indicesSolid; // COMBAK: does this have importance ?
  auto& _scheme = data.scheme;
  auto& _pos  = data.positions;
  auto& _norm = data.normals;
  auto& _uvs  = data.textureCoords;
  auto& _occl = data.occlusion;
  auto& _normm = data.normalMapCoords;

  for(size_t i = 0; i < 4; i++) {
    // indices
    _ind.insert(_ind.end(), _scheme.begin(), _scheme.end());
    std::transform(_scheme.begin(), _scheme.end(), _scheme.begin(), [](int x) { return x+4; });

    // positions
    auto& posFace = positions[i];
    _pos.insert(_pos.end(), posFace.begin(), posFace.end());
    for(int i = 0, k = 0; i < 12; i++, k = (k+1) % 3) {
      _pos[_pos.size() - 12 + i] += pos[k];
    }

    // normals
    auto const& normFace = normals[i];
    _norm.insert(_norm.end(), normFace.begin(), normFace.end());

    // textureCoords
    auto indexUV = block->getFaceUVs(BlockFace::FRONT);
    auto uvFace = computeUV(indexUV, faceUVs);
    _uvs.insert(_uvs.end(), uvFace.begin(), uvFace.end());

    // occlusion
    auto const& occl = occlusions[i];
    _occl.insert(_occl.end(), occl.begin(), occl.end());

    // normalMapCoords
    _normm.insert(_normm.end(), faceNormalMap.begin(), faceNormalMap.end());

  }
}

/// below is all the data and lookup tables

// the coordinates of x and y such that the diagonal face is of length 1
static const float off = sqrt(2.f) / 4.f;

const std::array<face_t<3>, 4> TallgrassModel::positions = {
  face_t<3>{ // face looking towards -x, +z
    -off,  0.f, -off,
     off,  0.f,  off,
     off, -0.5f,  off,
    -off, -0.5f, -off,
  },
  face_t<3>{ // face looking towards -x, -z
     off,  0.f, -off,
    -off,  0.f,  off,
    -off, -0.5f,  off,
     off, -0.5f, -off,
  },
  face_t<3>{ // face looking towards +x, -z
     off,  0.f,  off,
    -off,  0.f, -off,
    -off, -0.5f, -off,
     off, -0.5f,  off,
  },
  face_t<3>{ // face looking towards +x, +z
    -off,  0.f,  off,
     off,  0.f, -off,
     off, -0.5f, -off,
    -off, -0.5f,  off,
  },
};

const std::array<face_t<3>, 4> TallgrassModel::normals {
  face_t<3>{ // face looking towards -x, +z
    off,  0.f, -off,
    off,  0.f, -off,
    off,  0.f, -off,
    off,  0.f, -off,
  },
  face_t<3>{ // face looking towards -x, -z
    off,  0.f,  off,
    off,  0.f,  off,
    off,  0.f,  off,
    off,  0.f,  off,
  },
  face_t<3>{ // face looking towards +x, -z
    -off,  0.f,  off,
    -off,  0.f,  off,
    -off,  0.f,  off,
    -off,  0.f,  off,
  },
  face_t<3>{ // face looking towards +x, +z
    -off,  0.f, -off,
    -off,  0.f, -off,
    -off,  0.f, -off,
    -off,  0.f, -off,
  },
};

static const float occl = 2.f;

const std::array<std::array<GLfloat, 4>, 4> TallgrassModel::occlusions {
  std::array<GLfloat, 4>{
    0.f, 0.f, occl, occl
  },
  std::array<GLfloat, 4>{
    0.f, 0.f, occl, occl
  },
  std::array<GLfloat, 4>{
    0.f, 0.f, occl, occl
  },
  std::array<GLfloat, 4>{
    0.f, 0.f, occl, occl
  },
};

const face_t<2> TallgrassModel::faceNormalMap = {
    -1.0f, -1.0f,
    -1.0f, -1.0f,
    -1.0f, -1.0f,
    -1.0f, -1.0f,
};
