#include <iostream>
#include <memory>

#include "SDL2/SDL_image.h"

#include "Shader.hpp"
#include "ShadowMap.hpp"
#include "Viewport.hpp"
#include "terrain/Terrain.hpp"
#include "util/Loader.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (unsigned int)(1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))


int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";
    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    Terrain terrain;
    ShadowMap shadows(1024);
    Loader loader;

    GLuint textureID = loader.loadTexture("Testxture");

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        terrain.update(window.camera.position);

        // draw the shadow map
        float distance = 200.f;
        auto pos = window.camera.position + glm::normalize(glm::vec3(5, 4, 1)) * distance;
        shadows.update(pos, window.camera.position);
        shadows.beginFrame();
        terrain.render();
        shadows.endFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen
        shader.activate();
        window.camera.activate();

        GLint texSampler = shader.getUniformLocation("textureSampler");
        GLint shadowSampler = shader.getUniformLocation("shadowSampler");
        glUniform1i(texSampler, 0);
        glUniform1i(shadowSampler,  1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadows.getTextureID());

        // update the terrain
        shadows.activate();
        terrain.render();

        glBindTexture(GL_TEXTURE_2D, 0);

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
