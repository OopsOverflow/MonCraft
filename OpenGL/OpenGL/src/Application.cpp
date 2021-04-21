#include <iostream>
#include <memory>

#include "SDL2/SDL_image.h"

#include "Shader.hpp"
#include "ShadowMap.hpp"
#include "Viewport.hpp"
#include "terrain/Terrain.hpp"
#include "util/Loader.hpp"
#include "util/Raycast.hpp"
#include "entity/Hitbox.hpp"
#include "include/Music.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    800
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (unsigned int)(1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

#include "terrain/BlockGeom.hpp"
#include "blocks/Debug_Block.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

// TODO: temporary until I finish the skewed shadow matrix
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform2.hpp>

std::unique_ptr<Mesh> makeTargetBlock() {
  std::vector<GLfloat> positions;
  std::vector<GLfloat> normals;
  std::vector<GLfloat> textureCoords;
  std::vector<GLfloat> occlusion(4 * 6, 0);
  std::vector<GLuint> indices;
  std::vector<GLuint> scheme = { 0, 1, 2, 0, 2, 3 };

  auto block = Blocks::create_static<Debug_Block>();

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
    std::copy(blockPositions[i].begin(), blockPositions[i].end(), std::back_inserter(positions));
    std::copy(blockNormals[i].begin(), blockNormals[i].end(), std::back_inserter(normals));
    auto uvs = getFaceUV(block->getFaceUVs((BlockFace)i));
    std::copy(uvs.begin(), uvs.end(), std::back_inserter(textureCoords));
    std::copy(scheme.begin(), scheme.end(), std::back_inserter(indices));
    std::transform(scheme.begin(), scheme.end(), scheme.begin(),[](int x) { return x+4; });
  }

  Mesh* mesh = new Mesh(positions, normals, textureCoords, occlusion, indices);
  return std::unique_ptr<Mesh>(mesh);
}

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";

    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    Terrain terrain;
    Hitbox character({ 0.0f,32.0f,0.0f });
    ShadowMap shadows(1024);
    Loader loader;
    Raycast caster(100.f);
    std::unique_ptr<Mesh> targetBlock = makeTargetBlock();

    GLuint textureID = loader.loadTexture("Texture_atlas");
    character.cameraToHead(window.camera);

    Music MusicPlayer;

    int skyCamSize = 300;
    Camera skyCam(skyCamSize, skyCamSize, {1, 500, 1}, {0, 0, 0});

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        // updates
        window.keyboardController.apply(character);
        window.mouseController.apply(character, window.camera);
        character.cameraToHead(window.camera);

        MusicPlayer.update();

        auto playerPos = window.camera.position;
        auto viewDir = window.camera.center - window.camera.position;
        auto fovX = glm::degrees(2 * atan(tan(glm::radians(45.f) / 2) * window.width / window.height)); // see https://en.wikipedia.org/wiki/Field_of_view_in_video_games#Field_of_view_calculations
        terrain.update(playerPos, viewDir, fovX);

        auto castPos = window.camera.position;
        auto castDir = window.camera.center - window.camera.position;
        glm::vec3 castTarget = caster.cast(castPos, castDir, terrain);
        targetBlock->model = glm::translate(glm::mat4(1.f), castTarget);

        // draw the shadow map
        // float t = timeBegin / 10000.f;
        float t = glm::half_pi<float>();
        float distance = 100.f;
        float a = cos(t);
        float b = sin(t);
        if(b < 0) b = -b;
        auto sunPos = castTarget + glm::normalize(glm::vec3(a, b, a)) * distance;
        auto sunTarget = castTarget;
        shadows.update(sunPos, sunTarget);
        shadows.beginFrame();
        terrain.render();
        shadows.endFrame();

        // prepare render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.activate();
        window.camera.activate();

        // set light position / intensity
        glUniform1f(shader.getUniformLocation("lightIntensity"), 1);
        auto sunDir = window.camera.view * glm::normalize(-glm::vec4(a, b, a, 0.f));
        glUniform3fv(shader.getUniformLocation("lightDirection"), 1, glm::value_ptr(sunDir));

        // bind textures
        GLint texSampler = shader.getUniformLocation("textureSampler");
        GLint shadowSampler = shader.getUniformLocation("shadowSampler");
        glUniform1i(texSampler, 0);
        glUniform1i(shadowSampler,  1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadows.getTextureID());

        // draw target block
        glBindVertexArray(targetBlock->getVAO());
        glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(targetBlock->model));
        glDrawElements(GL_TRIANGLES, targetBlock->getVertexCount(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // draw the terrain
        shadows.activate();
        terrain.render();

        // terrain sky view
        glm::vec3 skyPos(window.camera.position.x, 500, window.camera.position.z);
        glm::vec3 skyCenter(window.camera.position.x, 0, window.camera.position.z - 1);
        skyCam.setLookAt(skyPos, skyCenter);
        skyCam.activate();
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glScissor(0, 0, skyCamSize + 5, skyCamSize + 5);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain.render();
        glScissor(skyCamSize/2-1, skyCamSize/2-1, 2, 2);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        // draw the character
        window.camera.activate();
        character.drawCharacter();

        // finish render
        window.endFrame();

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
        else if(timeEnd - timeBegin > 2 * TIME_PER_FRAME_MS) {
          std::cout << "can't keep up ! " << 1000.f / (timeEnd - timeBegin) << "fps" << std::endl;
        }
    }

    return 0;
}
