#pragma once

#include "../Camera.hpp"

class ControllerTurntable {
public:
  ControllerTurntable();

  void translateStart(int x, int y);
  void translateEnd(int x, int y);
  void rotateStart(int x, int y);
  void rotateEnd(int x, int y);
  void motion(int x, int y);
  void zoom(bool direction);

  static glm::vec3 screenToCameraPlane(const Camera &camera, int x, int y);

  void apply(Camera &camera);

private:
  int lastX;
  int lastY;
  int deltaX;
  int deltaY;
  bool translation;
  bool rotation;
  float zoomFactor;

  float speed;
};
