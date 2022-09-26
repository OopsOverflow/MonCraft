#include "PlayerController.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
        actionList.push_back(Action::ACTION_3_UP);
    }
    else if (k == config.view) {
        entity->view = (PlayerView)(((int)entity->view + 1) % 3);
    }
    else if (k == config.dab) {
        actionList.push_back(Action::ACTION_4_UP);
    }else if(k == config.menu) {
        if (direction.z != 0) direction.z = 0;
        if (direction.x != 0) direction.x = 0;
        if (direction.y != 0) direction.y = 0;
        actionList.push_back(Action::ACTION_3_UP);
        actionList.push_back(Action::ACTION_4_UP);
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
        actionList.push_back(Action::ACTION_3_DOWN);
    }
    else if (k == config.dab) {
        actionList.push_back(Action::ACTION_4_DOWN);
    }

    return true;
}

void PlayerController::update() {
    for(auto action: actionList) 
        entity->handleAction(action);
    actionList = {};

    if (toggleGod) {
        entity->god = !entity->god;
        entity->updateProperties();
        toggleGod = false;
    }

    float compY = direction.y;
    auto rotMatrix = glm::rotate(glm::highp_dmat4(1.0), entity->bodyNode.rot.y + entity->headNode.rot.y, {0, 1, 0});
    auto dir = glm::vec3(rotMatrix * glm::vec4(direction, 1.f));
    dir.y = compY;

    if(entity->god) {
        entity->walk(dir);
    }
    else {
        entity->walk({dir.x, 0, dir.z});
        if(direction.y > 0) entity->jump();
    }
}