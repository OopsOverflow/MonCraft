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

GLuint initHolyCube(Shader& shader) {

    GLfloat Positions[] = {
    //Top
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

    //Bottom
        0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

    //Front
       -0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

    //Right
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,

    //Back
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,

    //Left
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f

    };


    //Isn't checked, may have problems
    GLfloat Normals[] = {
    //Top
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

    //Bottom
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

    //Front
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

    //Right
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

    //Back
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

    //Left
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f

    };

    static const int TEXTURE_SIZE = 32;//Dynamic texture size
    static const int STACKED_TEXTURE = 2;//Can stack texture on a single image to load less images

    int textureNumber = 0;//texture Id for future implementation

    //calculate the value of the top and the bottom of the texture
    float botTexture = textureNumber * 1.0f / STACKED_TEXTURE;
    std::cout << "botTexture :" << botTexture << std::endl;
    float topTexture = (textureNumber + 1) * 1.0f / STACKED_TEXTURE;
    std::cout << "topTexture :" << topTexture << std::endl;

    GLfloat UVs[] = {
    //Top
        0.0  , topTexture,
        1.0/6, topTexture,
        1.0/6, botTexture,
        0.0  , topTexture,
        1.0/6, botTexture,
        0.0  , botTexture,

    //Bottom
        2*1.0/6, topTexture,
        1.0/6  , botTexture,
        1.0/6  , topTexture,
        2*1.0/6, topTexture,
        2*1.0/6, botTexture,
        1.0/6  , botTexture,

    //Front
        2*1.0/6, topTexture,
        3*1.0/6, botTexture,
        3*1.0/6, topTexture,
        2*1.0/6, topTexture,
        2*1.0/6, botTexture,
        3*1.0/6, botTexture, 

    //Right
        3*1.0/6, topTexture,
        4*1.0/6, botTexture,
        4*1.0/6, topTexture,
        3*1.0/6, topTexture,
        3*1.0/6, botTexture,
        4*1.0/6, botTexture,

    //Back
        5*1.0/6, topTexture,
        4*1.0/6, topTexture,
        4*1.0/6, botTexture, 
        5*1.0/6, topTexture,
        4*1.0/6, botTexture,
        5*1.0/6, botTexture,  

    //Left
        1.0    , topTexture,
        5*1.0/6, topTexture,
        5*1.0/6, botTexture,
        1.0    , topTexture,
        5*1.0/6, botTexture,
        1.0    , botTexture
    
    };

    // Genrate Buffer to draw the Cube
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Positions) +  sizeof(UVs), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Positions), Positions);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Positions), sizeof(UVs), UVs);

        GLint vPosition = shader.getLocation(ShaderLocation::VERTEX_POSITION);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(vPosition);

        GLint vUVs = 2;//To change
        glVertexAttribPointer(vUVs, 2, GL_FLOAT, 0, 0, (const void *)(sizeof(Positions)));
        glEnableVertexAttribArray(vUVs);


    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    return buffer;
}


int main(int argc, char* argv[]) {

    std::cout << "----Main------\n";
    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    GLuint buffer = initHolyCube(shader); // TODO: delete vao / vbo in the end

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

        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen

        shader.activate();
        window.camera.activate();

        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // actually draw the Holy Triangle
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glDrawArrays(GL_TRIANGLES, 0, 3*12);
        window.camera.rotate({ 0.5f,1.f,0.f }, false);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        window.endFrame();

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }

    return 0;
}
