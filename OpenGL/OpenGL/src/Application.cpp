#include <iostream>
#include <memory>

#include "SDL2/SDL_image.h"
#include "util/Loader.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (unsigned int)(1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))


// #include "DisplayNoise.hpp" // temporary file, to display the biome map
#include "terrain/Terrain.hpp"

GLuint initsky(Shader& shader) {
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(skyboxVertices), skyboxVertices);

        GLint vPosition = shader.getLocation(ShaderLocation::VERTEX_POSITION);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(vPosition);

    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";
    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");

    Shader skyBoxShader("src/shader/skyBox.vert", "src/shader/skyBox.frag");
    Loader skyTxr;
    std::vector<std::string> faces
    {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"
    };
    unsigned int cubemapTexture = skyTxr.loadCubeMap(faces);
    GLuint buffer = initsky(skyBoxShader);

    // DisplayNoise biomeMap;
    Terrain terrain;
    Loader blockTxr;
    GLuint textureID = blockTxr.loadTexture("Testxture");

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen

        // draw the biome map on top left corner
        // GLsizei size = (GLsizei)(window.height * .5);
        // glViewport((GLint)(window.width - size), (GLint)(window.height - size), size, size);
        // biomeMap.draw(); // comment this line to disable

        shader.activate();
        window.camera.activate();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // update the terrain
        terrain.render(window.camera);

        glBindTexture(GL_TEXTURE_2D, 0);

        window.endFrame();

        // draw skybox at last
        glDepthFunc(GL_LEQUAL);
        skyBoxShader.activate();
        glBindVertexArray(buffer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

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
