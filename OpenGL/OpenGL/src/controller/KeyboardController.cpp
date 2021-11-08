#include "KeyboardController.hpp"

#include <glm/glm.hpp>

#include "save/ClientConfig.hpp"
#include "terrain/World.hpp"

KeyboardController::KeyboardController()
  : config(Config::getClientConfig()),
    direction(0.f),
    state(State::Idle),
    view(CharacterView::FIRST_PERSON),
    sprint(false), toggleGod(false),
    spaceIsPressed(false), lastSpacePress(0.f)
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

    return true;
}

bool KeyboardController::isDoubleSpace() {
  static const float threshold = 0.3f;
  bool res = false;

  if(!spaceIsPressed) {
    auto time = World::getInst().t;
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

    return true;
}

void KeyboardController::apply(Character& character) {
  character.view = view;
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
