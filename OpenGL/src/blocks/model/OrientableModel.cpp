#include "OrientableModel.hpp"

#include <glm/glm.hpp>
#include <algorithm>
#include <utility>
#include <vector>

#include "blocks/Block.hpp"
#include "blocks/Orientable_Block.hpp"
#include "blocks/model/DefaultModel.hpp"
#include "gl/Mesh.hpp"

using namespace glm;

OrientableModel::OrientableModel(QuadMesh<3> mesh, QuadMesh<2> UVMesh)
  : mesh(mesh),
    UVMesh(UVMesh),
    orientedMesh(computeFacing(mesh)),
    orientedUVMesh(computeFacing(normals(mesh)))
{}

OrientableModel* OrientableModel::get() {
  static const QuadMesh<2> UVMesh = {
    std::vector<Quad<2>>
    { faceUVs },
    { faceUVs },
    { faceUVs },
    { faceUVs },
    { faceUVs },
    { faceUVs },
    { }, // INNER
  };
  static OrientableModel inst(DefaultBlockModel::mesh, UVMesh);
  return &inst;
}

static const std::array<std::array<size_t, 7>, 4> facingLookup {
  // TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT, INNER
  std::array<size_t, 7>
  { 0, 1, 2, 3, 4, 5, 6 }, // NORTH
  { 0, 1, 4, 5, 2, 3, 6 }, // SOUTH
  { 0, 1, 3, 4, 5, 2, 6 }, // EAST
  { 0, 1, 5, 2, 3, 4, 6 }, // WEST
};

void OrientableModel::genFace(glm::ivec3 pos, BlockFace face, Orientable_Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
    auto& _ind = data.indicesSolid;
    auto& _scheme = data.scheme;
    auto& _pos  = data.positions;
    auto& _norm = data.normals;
    auto& _uvs  = data.textureCoords;
    auto& _occl = data.occlusion;
    auto& _normm = data.normalMapCoords;

    Facing facing = block->getFacing();
    size_t flookup = facingLookup[(size_t)facing][(size_t)face];

    // indices
    size_t quadCount = mesh[flookup].size();
    for(size_t i = 0; i < quadCount; i++) {
      _ind.insert(_ind.end(), _scheme.begin(), _scheme.end());
      std::transform(_scheme.begin(), _scheme.end(), _scheme.begin(), [](int x) { return x+4; });
    }

    // positions
    auto& posFace = orientedMesh[(size_t)facing][flookup];
    _pos.insert(_pos.end(), posFace.begin(), posFace.end());
    for(size_t i = 0, k = 0; i < posFace.size(); i++, k = (k+1) % 3) {
      _pos[_pos.size() - posFace.size() + i] += pos[k];
    }

    // normals
    auto const& normFace = orientedUVMesh[(size_t)facing][flookup];
    _norm.insert(_norm.end(), normFace.begin(), normFace.end());

    // textureCoords
    auto indexUV = block->getFaceUVs(face);
    auto uvFace = computeUV(indexUV, UVMesh[flookup]);
    _uvs.insert(_uvs.end(), uvFace.begin(), uvFace.end());

    // occlusion
    if(face == BlockFace::INNER) {
      for(size_t i = 0; i < quadCount; i++) {
        static const FaceData<1> empty{};
        _occl.insert(_occl.end(), empty.begin(), empty.end());
      }
    }
    else {
      auto occl = genOcclusion(pos, neighbors, face);
      auto const& quads = mesh[flookup];
      for(auto quad : quads) {
        if (face == BlockFace::TOP || face == BlockFace::BOTTOM) {
          quad = transform(quad, facingTransforms[(size_t)facing]);
        }
        quad = transform(quad, faceTransforms[flookup]);
        for(vec2 pos : quad) {
          _occl.push_back(lerp(occl, pos));
        }
      }
    }

    // normalMapCoords
    for(size_t i = 0; i < quadCount; i++) {
      _normm.insert(_normm.end(), faceNormalMap.begin(), faceNormalMap.end());
    }
}

void OrientableModel::generateMesh(ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  auto orient = static_cast<Orientable_Block*>(block);

  for(auto const& off : blockFaceOffsets) {
    auto neigh = neighbors[off.first];

    if(neigh->isOpaque())
      continue;

    else if(block->isTransparent() && block->type == neigh->type)
      continue;

    else
      genFace(pos, off.second, orient, neighbors, data);
  }

  genFace(pos, BlockFace::INNER, orient, neighbors, data);
}
