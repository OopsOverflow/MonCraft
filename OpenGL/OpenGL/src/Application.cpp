#include <iostream>
#include <memory>

#include "SDL2/SDL_image.h"

#include "Shader.hpp"
#include "Viewport.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (unsigned int)(1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

#include "DisplayNoise.hpp" // temporary file, to display the biome map
#include "terrain/Terrain.hpp"

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";
    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");

    // DisplayNoise biomeMap;
    Terrain terrain;

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Could not load SDL2 image with PNG files\n";
        return EXIT_FAILURE;
    }

    SDL_Surface* img = IMG_Load("src/blocks/Textures/Testxture.png");
    SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(img);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgbImg->w, rgbImg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)rgbImg->pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    SDL_FreeSurface(rgbImg);

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen

        // draw the biome map on top left corner
        // int size = window.height * .5;
        // glViewport(window.width - size, window.height - size, size, size);
        // biomeMap.draw(); // comment this line to disable

        shader.activate();
        // window.camera.rotate({ 0.5f,1.f,0.f }, false);
        window.camera.activate();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // update the terrain
        terrain.render(window.camera);

        glBindTexture(GL_TEXTURE_2D, 0);

        window.endFrame();

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
        else if(timeEnd - timeBegin > 2 * TIME_PER_FRAME_MS) {
          std::cout << "can't keep up ! " << timeEnd - timeBegin - TIME_PER_FRAME_MS << "ms behind" << std::endl;
        }
    }

    return 0;
}
