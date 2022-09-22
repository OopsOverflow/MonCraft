#include "PlayerController.hpp"

#include <glm/glm.hpp>
#include "save/ClientConfig.hpp"
#include <SDL2/SDL.h>

PlayerController::PlayerController(std::shared_ptr<Entity> entity)
: Controller(entity), toggleGod(false),
    spaceIsPressed(false), lastSpacePress(SDL_GetTicks()),
    direction(0)
{}

bool PlayerController::handleKeyReleased(Key k) {
    auto config = Config::getClientConfig();
    if (k == config.forward) {
        if (direction.z == -1) direction.z = 0;
    }
    else if (k == config.backward) {
        if (direction.z == 1) direction.z = 0;
     }
    else if (k == config.right) {
        if (direction.x == 1) direction.x = 0;
    }
    else if (k == config.left) {
        if (direction.x == -1) direction.x = 0;
    }
    else if (k == config.jump) {
        if (direction.y == 1) direction.y = 0;
        spaceIsPressed = false;
    }
    else if (k == config.sneak) {
        if (direction.y == -1) direction.y = 0;
    }
    else if (k == config.sprint) {
        entity->setSprint(false);
    }
    else if (k == config.view) {
        entity->view = (PlayerView)(((int)entity->view + 1) % 3);
    }
    else if (k == config.dab) {
        entity->dab = false;
    }else if(k == config.menu) {
        if (direction.z != 0) direction.z = 0;
        if (direction.x != 0) direction.x = 0;
        if (direction.y != 0) direction.y = 0;
        entity->sprint = false;
        entity->dab = false;
        spaceIsPressed = false;
    }

    return true;
}

bool PlayerController::isDoubleSpace() {
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


bool PlayerController::handleKeyPressed(Key k) {
    auto config = Config::getClientConfig();
    if (k == config.forward) {
        direction.z = -1;
    }
    else if(k == config.backward){
        direction.z = 1;
    }
    else if (k == config.right) {
        direction.x = 1;
    }
    else if (k == config.left) {
        direction.x = -1;
    }
    else if (k == config.jump) {
        if(isDoubleSpace()) toggleGod = true;
        else direction.y = 1;
    }
    else if (k == config.sneak) {
        direction.y = -1;
    }
    else if (k == config.sprint) {
        entity->setSprint(true);
    }
    else if (k == config.dab) {
        entity->dab = true;
    }

    return true;
}

void PlayerController::update() {
  if (toggleGod) {
      entity->toggleGodMode();
      toggleGod = false;
  }
  if(entity->god) {
    entity->walk(direction);
  }
  else {
    entity->walk({direction.x, 0, direction.z});
    if(direction.y > 0) entity->jump();
  }
}