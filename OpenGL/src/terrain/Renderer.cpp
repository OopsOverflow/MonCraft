#include "Renderer.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "gl/Shader.hpp"
#include "terrain/ChunkMesh.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"

class Chunk;

using namespace glm;

Renderer::Renderer()
  : world(World::getInst())
{}

ChunkList Renderer::visibleChunks(Camera const& camera) {
  std::vector<std::pair<float, std::shared_ptr<ChunkMesh>>> res;

  world.chunks.for_each([&](std::shared_ptr<ChunkMesh> chunk) {
    vec3 worldChunkPos = vec3(chunk->chunkPos * world.chunkSize);
    vec3 chunkCenter = worldChunkPos + vec3(world.chunkSize) / 2.f;
    vec4 posCamSpace = camera.view * vec4(chunkCenter, 1.0f);
    static const float tolerance = world.chunkSize * .5f * sqrt(3.f);

    if(camera.chunkInView(posCamSpace, tolerance)) {
      res.emplace_back(-posCamSpace.z, chunk);
    }
  });

  // sort back-to-front (far chunks first)
  std::sort(res.begin(), res.end(), [](auto const& a, auto const& b) {
    return a.first > b.first;
  });

  
  return res;
}

void Renderer::render(Camera const& camera, ChunkList const& chunks) {
  // draw solid
  for (auto iter = chunks.rbegin(); iter != chunks.rend(); ++iter) {
    auto& pair = *iter;
    pair.second->drawSolid();
  }

  // draw transparent
  Shader* shader = Shader::getActive(); // TODO: find a better way
  GLint flags;
  glGetUniformiv(shader->program, shader->getUniform("flags"), &flags);
  flags = (flags & 0b1101) | 0b0010;
  glUniform1i(shader->getUniform("flags"), flags);

  auto viewDir = camera.center - camera.position;
  for(auto& pair : chunks) {
    pair.second->drawTransparent(viewDir);
  }

  flags = (flags & 0b1101) | 0b0000;
  glUniform1i(shader->getUniform("flags"), flags);
}

void Renderer::renderSolid(ChunkList const& chunks) {
  for(auto& pair : chunks) {
    pair.second->drawAllAsSolid();
  }
}
