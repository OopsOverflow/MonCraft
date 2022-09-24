#include "Renderer.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

#include "gl/Shader.hpp"
#include "terrain/ChunkMesh.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"
#include "gl/ResourceManager.hpp"

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

void Renderer::renderWireframe(Camera const& camera) {
  GLuint vao;
  GLuint vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  std::vector<GLfloat> data;
  
  world.chunks.for_each([&] (std::shared_ptr<ChunkImpl> chunk) {
    glm::vec3 p1 = chunk->chunkPos * 16;
    glm::vec3 p2 = (chunk->chunkPos + 1) * 16;
    p1 -= .5f;
    p2 -= .5f;
    data.insert(data.end(), { p1.x, p1.y, p1.z, p2.x, p1.y, p1.z });
    data.insert(data.end(), { p1.x, p1.y, p1.z, p1.x, p2.y, p1.z });
    data.insert(data.end(), { p1.x, p1.y, p1.z, p1.x, p1.y, p2.z });
  });
  
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(VERTEX_POSITION);
  
  Shader* shader = ResourceManager::getShader("wireframe");
  shader->activate();
  camera.activate();
  glm::mat4 model(1.f);
  glUniformMatrix4fv(shader->getUniform(MATRIX_MODEL), 1, GL_FALSE, glm::value_ptr(model));
  
  glDrawArrays(GL_LINES, 0, data.size() / 3);
  
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}
