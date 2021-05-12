#include "Viewport.hpp"

//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// GLEW Libraries
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

const int Viewport::framerate = 60;
const int Viewport::timePerFrame = 1000 / framerate;

Viewport::Viewport(size_t width, size_t height)
    : camera(width, height, {0, 32, 10}, {0, 32, 0}),
      width(width),
      height(height),
      window(nullptr),
      context(nullptr),
      lastSpacePress(0), timeBegin(0), lastTime(0),
      mouseCaptured(false), vsync(true)
{

  //Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
      throw std::runtime_error(std::string("SDL init failed: ") + SDL_GetError());
  }

  //Create a Window
  window = SDL_CreateWindow("MonCraft",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  //Initialize the OpenGL Context
  context = SDL_GL_CreateContext(window);

  //Check GLEW Intitialisation
  if (glewInit() != GLEW_OK)
    throw std::runtime_error("GLEW init failed");

  std::cout << "---------" << std::endl;
  std::cout << "OpenGL  :" << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL    :" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor  :" << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer:" << glGetString(GL_RENDERER) << std::endl;
  std::cout << "---------" << std::endl;
}

Viewport::~Viewport() {
    //Free everything
    if (context)
        SDL_GL_DeleteContext(context);
    if (window)
        SDL_DestroyWindow(window);
}

void Viewport::on_event(SDL_Event const& e) {
  switch (e.type) {
  case SDL_WINDOWEVENT:
    on_window_event(e.window);
    break;
  case SDL_KEYDOWN:
    on_keydown(e.key.keysym.sym);
    break;
  case SDL_KEYUP:
    on_keyup(e.key.keysym.sym);
    break;
  case SDL_MOUSEMOTION:
    mouseController.motionRel(e.motion.xrel, e.motion.yrel);
    break;
  case SDL_MOUSEBUTTONDOWN:
    on_mousedown(e.button);
    break;
  case SDL_MOUSEBUTTONUP:
    on_mouseup(e.button);
    break;
  }
}

bool Viewport::beginFrame(float& dt) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if(event.type == SDL_WINDOWEVENT)
      if(event.window.event == SDL_WINDOWEVENT_CLOSE)
        return false;
    if(event.type == SDL_QUIT)
      return false;
    on_event(event);
  }

  glClearColor(54/255.f, 199/255.f, 242/255.f, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  timeBegin = SDL_GetTicks();
  dt = (timeBegin - lastTime) / 1000.f;

  return true;
}

void Viewport::endFrame() {
  //Display on screen (swap the buffer on screen and the buffer you are drawing on)
  SDL_GL_SwapWindow(window);

  //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
  uint32_t timeEnd = SDL_GetTicks();
  lastTime = timeBegin;

  if(!vsync) std::cout << "fps: " << 1000.f / (timeEnd - timeBegin) << std::endl;
  if (timeEnd - timeBegin < timePerFrame) {
    if(vsync) SDL_Delay(timePerFrame - (timeEnd - timeBegin));
  }
  else if(timeEnd - timeBegin > 2 * timePerFrame) {
    std::cout << "can't keep up ! " << 1000.f / (timeEnd - timeBegin) << "fps" << std::endl;
  }
}

void Viewport::on_window_event(SDL_WindowEvent const& e) {
  switch (e.event) {
  case SDL_WINDOWEVENT_SIZE_CHANGED:
    width = e.data1;
    height = e.data2;
    camera.setSize(width, height);
    break;
  }
}

void Viewport::on_keydown(SDL_Keycode k) {
  switch (k) {
  case SDLK_z:
    keyboardController.pressedForward();
    break;
  case SDLK_s:
    keyboardController.pressedBackward();
    break;
  case SDLK_d:
    keyboardController.pressedRight();
    break;
  case SDLK_q:
    keyboardController.pressedLeft();
    break;
  case SDLK_SPACE:
  {
      uint32_t currentTime = SDL_GetTicks();
      if (currentTime - lastSpacePress > 100 && currentTime - lastSpacePress < 250) {
          keyboardController.changedMod();
      }
      else {
          keyboardController.pressedUp();
      }
      lastSpacePress = currentTime; }

    break;
  case SDLK_LSHIFT:
    keyboardController.pressedDown();
    break;
  case SDLK_F5:
      keyboardController.pressedF5();
      break;
  case SDLK_LCTRL:
      keyboardController.pressedControl();
      break;
  case SDLK_ESCAPE:
      SDL_SetRelativeMouseMode(SDL_FALSE);
      int x, y;
      SDL_GetMouseState(&x, &y);
      mouseController.rotateEnd(x, y);
      mouseCaptured = false;
      break;
  case SDLK_p:
      keyboardController.pressedPause();
      break;
  case SDLK_f:
    vsync = !vsync;
      if(vsync) SDL_GL_SetSwapInterval(1);
      else SDL_GL_SetSwapInterval(0);
      break;
  }
}

void Viewport::on_keyup(SDL_Keycode k) {
  switch (k) {
  case SDLK_z:
    keyboardController.releasedForward();
    break;
  case SDLK_s:
    keyboardController.releasedBackward();
    break;
  case SDLK_d:
    keyboardController.releasedRight();
    break;
  case SDLK_q:
    keyboardController.releasedLeft();
    break;
  case SDLK_SPACE:
    keyboardController.releasedUp();
    break;
  case SDLK_LSHIFT:
    keyboardController.releasedDown();
    break;
  case SDLK_LCTRL:
      keyboardController.releasedControl();
      break;
  }
}

void Viewport::on_mousedown(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::DESTROY);
    }
    else {
      SDL_SetRelativeMouseMode(SDL_TRUE);
      mouseController.rotateStart(e.x, e.y);
      mouseCaptured = true;
    }
    break;
  case SDL_BUTTON_RIGHT:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::PLACE);
    }
    break;
  default:
    break;
  }
}

void Viewport::on_mouseup(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
      // mouseController.rotateEnd(e.x, e.y);
    break;
  default:
    break;
  }
}
