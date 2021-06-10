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

// force use of dedicated gpu on Nvidia optimus laptops / Amd laptops
#ifdef _WIN32
extern "C" {
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

Viewport::Viewport(glm::ivec2 size)
  :   size(size),
      window(nullptr), context(nullptr),
      lastSpacePress(0), spaceIsPressed(false),
      timeBegin(0), lastTime(0),
      mouseCaptured(false), vsync(true),
      root(nullptr)
{
  //Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error(std::string("SDL init failed: ") + SDL_GetError());
  }

  //Create a Window
  window = SDL_CreateWindow("MonCraft",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      size.x, size.y,
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

void Viewport::createRoot() {
  root = std::make_unique<ui::Root>(size);
}

Viewport::~Viewport() {
    //Free everything
    if (context)
        SDL_GL_DeleteContext(context);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

ui::Root* Viewport::getRoot() {
  return root.get();
}

void Viewport::captureMouse() {
  SDL_SetRelativeMouseMode(SDL_TRUE);
  mouseCaptured = true;
  int x, y;
  SDL_GetMouseState(&x, &y);
  mouseController.rotateStart(x, y);
}

void Viewport::toggleVSync() {
  vsync = !vsync;
  if(vsync) SDL_GL_SetSwapInterval(1);
  else SDL_GL_SetSwapInterval(0);
}

void Viewport::toggleFullscreen() {
  fullscreen = !fullscreen;
  if(fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  else SDL_SetWindowFullscreen(window, 0);
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
    if(mouseCaptured) {
      mouseController.motionRel(e.motion.xrel, e.motion.yrel);
    }
    else {
      root->addEvent(Event(Event::Type::MOVE, {e.motion.x, size.y - e.motion.y}));
    }
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
  root->update();
  root->render();

  //Display on screen (swap the buffer on screen and the buffer you are drawing on)
  SDL_GL_SwapWindow(window);

  //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
  uint32_t timeEnd = SDL_GetTicks();
  lastTime = timeBegin;

  if (vsync && timeEnd - timeBegin < timePerFrame) {
    SDL_Delay(timePerFrame - (timeEnd - timeBegin));
  }
}

void Viewport::on_window_event(SDL_WindowEvent const& e) {
  switch (e.event) {
  case SDL_WINDOWEVENT_SIZE_CHANGED:
      size.x = e.data1;
      size.y = e.data2;
      root->setSize(size);
      break;
  }
}

bool Viewport::isDoubleSpace() {
  static const int thresold = 300;

  bool res = false;

  if(!spaceIsPressed) {
    uint32_t time = SDL_GetTicks();
    res = time - lastSpacePress < thresold;
    lastSpacePress = time;
    spaceIsPressed = true;
  }

  return res;
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
    if(isDoubleSpace()) keyboardController.changedMod();
    else keyboardController.pressedUp();
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
      toggleVSync();
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
    spaceIsPressed = false;
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
      root->addEvent(Event(Event::Type::PRESS, {e.x, size.y - e.y}));
    }
    break;
  case SDL_BUTTON_RIGHT:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::PLACE);
    }
    break;
  case SDL_BUTTON_MIDDLE:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::PICK);
    }
    break;
  default:
    break;
  }
}

void Viewport::on_mouseup(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
      if(!mouseCaptured) {
        root->addEvent(Event(Event::Type::RELEASE, {e.x, size.y - e.y}));
      }
    break;
  default:
    break;
  }
}
