#include "controller/MouseController.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

MouseController::MouseController() {
  lastX = lastY = deltaX = deltaY = 0;
  speed = 0.f;
  rotation = false;
}


void MouseController::rotateStart(int x, int y) {
    lastX = x;
    lastY = y;
    rotation = true;
}

void MouseController::rotateEnd(int x, int y) {
  if (rotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
    rotation = false;
  }
}

void MouseController::motion(int x, int y) {
    if (rotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
  }
}

void MouseController::apply(Hitbox& character, Camera& camera) {

  unsigned int screenWidth;
  unsigned int screenHeight;

  float maxRotation = 360.f;
  camera.getSize(screenWidth, screenHeight);
  if (rotation) {
      std::vector<glm::vec3> vec = camera.getBoxCorners();
      //std::cout << camera.position.x<<", "<<camera.position.y<<", "<<camera.position.z << std::endl;

      glm::vec3 vecS = glm::vec3(camera.view * glm::vec4(vec.at(4), 1.0f));
      //std::cout << ((float)vec.at(4).x + vec.at(7).x) * 0.5 << ", " << ((float)vec.at(4).y + vec.at(7).y) * 0.5 << ", " << ((float)vec.at(4).z + vec.at(7).z) * 0.5 << std::endl;
      //std::cout << vecS.x << ", " << vecS.y << ", " << vecS.z << std::endl;
      //std::cout << "---------------" << std::endl;
      character.character.rotateHead({ -deltaY * maxRotation/ (float)screenWidth,-deltaX * maxRotation / (float)screenWidth });
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

}