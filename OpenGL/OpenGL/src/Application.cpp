//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include "Camera.hpp"
#include "Shader.hpp"

//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// GLEW Libraries
#include <GL/glew.h>

#include <iostream>

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

int main(int argc, char* argv[]) {


    //Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        // ERROR("The initialization of the SDL failed : %s\n", SDL_GetError());
        return 0;
    }

    //Create a Window
    SDL_Window* window = SDL_CreateWindow("MonCraft",                           //Titre
        SDL_WINDOWPOS_UNDEFINED,               //X Position
        SDL_WINDOWPOS_UNDEFINED,               //Y Position
        WIDTH, HEIGHT,                         //Resolution
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN); //Flags (OpenGL + Show)


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    //Initialize the OpenGL Context
    SDL_GLContext context = SDL_GL_CreateContext(window);

    //Check GLEW Intitialisation
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Error" << std::endl;
    }


    //Start using OpenGL to draw something on screen
    glViewport(0, 0, WIDTH, HEIGHT); //Draw on ALL the screen

    //The OpenGL background color (RGBA, each component between 0.0f and 1.0f)
    glClearColor(0.0, 0.0, 0.0, 1.0); //Full Black

    // Postion of Vertices
    float positions[]{
    -0.5f, -0.5f,
     0.0f,  0.5f,
     0.5f, -0.5f
    };

    // Genrate Buffer to draw the Triangle
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Shader shader("shader/simple.vert", "shader/simple.frag");
    Camera camera(WIDTH, HEIGHT, {0, 0, 10}, {0, 0, 0});

    // GPU Driver Version
    std::cout << glGetString(GL_VERSION) << std::endl;


    // Loop until the user closes the window
    bool isOpened = true;
    while (isOpened)
    {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        //Fetch the SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_CLOSE:
                    isOpened = false;
                    break;
                default:
                    break;
                }
                break;
                //We can add more event, like listening for the keyboard or the mouse. See SDL_Event documentation for more details
            }
        }

        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);

        shader.activate();
        camera.rotate({0, 1, 0});
        camera.activate();

        // Draw the Holy Triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(0); //Close the program. This is heavy for the GPU. In reality we do this

        //Display on screen (swap the buffer on screen and the buffer you are drawing on)
        SDL_GL_SwapWindow(window);

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }

    //Free everything
    if (context != NULL)
        SDL_GL_DeleteContext(context);
    if (window != NULL)
        SDL_DestroyWindow(window);

    return 0;

}
