#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <memory>

#include "../terrain/BlockGeom.hpp"
#include "../blocks/Debug_Block.h"
#include "../terrain/Chunk.hpp"

static std::unique_ptr<Mesh> makeDebugBlock() {
  std::vector<GLfloat> positions;
  std::vector<GLfloat> normals;
  std::vector<GLfloat> textureCoords;
  std::vector<GLfloat> occlusion(4 * 6, 0);
  std::vector<GLuint> indices;
  std::vector<GLuint> scheme = { 0, 1, 2, 0, 2, 3 };

  auto block = Block::create_static<Debug_Block>();

   auto getFaceUV = [](glm::ivec2 index) -> face_t<2>{
    static const float atlasSize = 8.f;
    return face_t<2> {
      (index.x + 1) / atlasSize, (index.y + 0) / atlasSize,
      (index.x + 0) / atlasSize, (index.y + 0) / atlasSize,
      (index.x + 0) / atlasSize, (index.y + 1) / atlasSize,
      (index.x + 1) / atlasSize, (index.y + 1) / atlasSize,
    };
  };

  for(int i = 0; i < 6; i++) {
    positions.insert(positions.begin(), blockPositions[i].begin(), blockPositions[i].end());
    normals.insert(normals.begin(), blockNormals[i].begin(), blockNormals[i].end());
    auto uvs = getFaceUV(block->getFaceUVs((BlockFace)i));
    textureCoords.insert(textureCoords.begin(), uvs.begin(), uvs.end());
    indices.insert(indices.begin(), scheme.begin(), scheme.end());
    std::transform(scheme.begin(), scheme.end(), scheme.begin(),[](int x) { return x+4; });
  }

  Mesh* mesh = new Mesh(positions, normals, textureCoords, occlusion, indices);
  return std::unique_ptr<Mesh>(mesh);
}
