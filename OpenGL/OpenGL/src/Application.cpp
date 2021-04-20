#include <iostream>
#include <memory>

#include "SDL2/SDL_image.h"

#include "Shader.hpp"
#include "Viewport.hpp"
#include "entity/Hitbox.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (unsigned int)(1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))


// #include "DisplayNoise.hpp" // temporary file, to display the biome map
#include "terrain/Terrain.hpp"

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";
    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");

    // DisplayNoise biomeMap;
    Terrain terrain;
    Hitbox character;

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen

        // draw the biome map on top left corner
        // GLsizei size = (GLsizei)(window.height * .5);
        // glViewport((GLint)(window.width - size), (GLint)(window.height - size), size, size);
        // biomeMap.draw(); // comment this line to disable

        shader.activate();
        // window.camera.rotate({ 0.5f,1.f,0.f }, false);
        window.camera.activate();

        // update the terrain
        terrain.render(window.camera);

        character.drawCharacter(window.camera);


        window.endFrame();

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
        else if(timeEnd - timeBegin > 2 * TIME_PER_FRAME_MS) {
          std::cout << "can't keep up ! " << timeEnd - timeBegin << "ms per frame" << std::endl;
        }
    }

    return 0;
}
