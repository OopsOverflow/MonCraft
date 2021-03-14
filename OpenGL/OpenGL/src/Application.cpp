#include <iostream>
#include <memory>

#include "Shader.hpp"
#include "Viewport.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

GLuint initHolyTriangle(Shader& shader) {

    float Positions[] = { // Postion of Vertices
      -1.0, -1.0, 0.0,
       1.0, -1.0, 0.0,
       0.0, 1.0, 0.0
    };

    float Color[] = {
      -1.0, -1.0, 1.0,
       1.0, -1.0, 1.0,
       1.0, 1.0, -1.0
    };

    // Genrate Buffer to draw the Triangle
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    {
        glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 9 * sizeof(float), Positions);
        glBufferSubData(GL_ARRAY_BUFFER, 9 * sizeof(float), 9 * sizeof(float), Color);

        GLint vPosition = shader.getLocation(ShaderLocation::VERTEX_POSITION);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(vPosition);

        GLint vColor = shader.getLocation(ShaderLocation::VERTEX_COLOR);
        glVertexAttribPointer(vColor, 3, GL_FLOAT, 0, 0, (const void *)(9 * sizeof(float)));
        glEnableVertexAttribArray(vColor);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

int main(int argc, char* argv[]) {

    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    GLuint buffer = initHolyTriangle(shader); // TODO: delete vao / vbo in the end

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT); // Clear Screen

        shader.activate();
        window.camera.activate();

        // actually draw the Holy Triangle
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        window.endFrame();

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }

    return 0;
}
