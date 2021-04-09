#include <iostream>
#include <memory>

#include "Shader.hpp"
#include "Viewport.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (unsigned int)(1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

GLuint initHolyCube(Shader& shader) {

    GLfloat Positions[] = {
        -0.5f,-0.5f,-0.5f, // triangle 1 : begin
        -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, // triangle 1 : end
        0.5f, 0.5f,-0.5f, // triangle 2 : begin
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f, // triangle 2 : end
        0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f,-0.5f, 0.5f
    };


    GLfloat Color[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
    };

    // Genrate Buffer to draw the Cube
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Positions) + sizeof(Color), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Positions), Positions);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Positions), sizeof(Color), Color);

        GLint vPosition = shader.getLocation(ShaderLocation::VERTEX_POSITION);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(vPosition);

        GLint vColor = shader.getLocation(ShaderLocation::VERTEX_COLOR);
        glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, (const void *)(sizeof(Positions)));
        glEnableVertexAttribArray(vColor);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

#include "DisplayNoise.hpp"

int main(int argc, char* argv[]) {

    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    GLuint buffer = initHolyCube(shader); // TODO: delete vao / vbo in the end
    DisplayNoise biomeMap;

    while (window.beginFrame()) {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen

        shader.activate();
        window.camera.activate();

        // actually draw the Holy Triangle
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glDrawArrays(GL_TRIANGLES, 0, 3*12);
        window.camera.rotate({ 0.5f,1.f,0.f }, false);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // draw the biome map on top left corner
        int size = window.height * .5;
        glViewport(window.width - size, window.height - size, size, size);
        biomeMap.draw();

        window.endFrame();

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }

    return 0;
}
