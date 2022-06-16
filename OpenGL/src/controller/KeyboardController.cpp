#include "KeyboardController.hpp"

#include <glm/glm.hpp>

#include "save/ClientConfig.hpp"
#include "terrain/World.hpp"
#include <SDL2/SDL.h>

KeyboardController::KeyboardController()
  : config(Config::getClientConfig()),
    direction(0.f),
    view(CharacterView::FIRST_PERSON),
    sprint(false), toggleGod(false), dab(false),
    spaceIsPressed(false), lastSpacePress(SDL_GetTicks())
{}

bool KeyboardController::handleKeyReleased(Key k) {
    auto code = k.asKeycode();
    if (code == config.forward) {
        if (direction.z == -1) direction.z = 0;
    }
    else if (code == config.backward) {
        if (direction.z == 1) direction.z = 0;
     }
    else if (code == config.right) {
        if (direction.x == 1) direction.x = 0;
    }
    else if (code == config.left) {
        if (direction.x == -1) direction.x = 0;
    }
    else if (code == config.jump) {
        if (direction.y == 1) direction.y = 0;
        spaceIsPressed = false;
    }
    else if (code == config.sneak) {
        if (direction.y == -1) direction.y = 0;
    }
    else if (code == config.sprint) {
        sprint = false;
    }
    else if (code == config.view) {
            view = CharacterView(((int)view + 1) % 3);
    }
    else if (code == config.dab) {
        dab = false;
    }

    return true;
}

bool KeyboardController::isDoubleSpace() {
  static const uint32_t threshold = 300;
  bool res = false;

  if(!spaceIsPressed) {
    auto time = SDL_GetTicks();
    res = time - lastSpacePress < threshold;
    lastSpacePress = time;
    spaceIsPressed = true;
  }

  return res;
}


bool KeyboardController::handleKeyPressed(Key k) {
    auto code = k.asKeycode();
    if (code == config.forward) {
        direction.z = -1;
    }
    else if(code == config.backward){
        direction.z = 1;
    }
    else if (code == config.right) {
        direction.x = 1;
    }
    else if (code == config.left) {
        direction.x = -1;
    }
    else if (code == config.jump) {
        if(isDoubleSpace()) toggleGod = true;
        else direction.y = 1;
    }
    else if (code == config.sneak) {
        direction.y = -1;
    }
    else if (code == config.sprint) {
        sprint = true;
    }
    else if (code == config.dab) {
        dab = true;
    }

    return true;
}

void KeyboardController::apply(Character& character) {
  character.view = view;
  character.setDab(dab);
  character.setSprint(sprint);
  if (toggleGod) {
      character.toggleGodMode();
      toggleGod = false;
  }
  if(character.getGodMode()) {
    character.walk(direction);
  }
  else {
    character.walk({direction.x, 0, direction.z});
    if(direction.y > 0) character.jump();
  }
}
