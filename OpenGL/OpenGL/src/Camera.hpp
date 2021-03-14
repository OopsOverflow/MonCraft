#pragma once

#include <glm/glm.hpp>

enum Projection { PROJECTION_ORTHOGRAPHIC,
                  PROJECTION_PERSPECTIVE };

class Camera {
public:
  Camera(uint width, uint height, const glm::vec3 &position,
         const glm::vec3 &center, Projection proj = PROJECTION_PERSPECTIVE);

  void activate();

  void setSize(uint width, uint height);
  void setProjectionType(Projection projType);
  void setPosition(const glm::vec3 &position);
  void setLookAt(const glm::vec3 &lookAt);

  void zoom(float factor);

  void translate(const glm::vec3 &direction, bool localSpace = false);
  void translatePixels(int x, int y);

  void rotate(const glm::vec3 &rotation, bool localSpace = false);
  void rotatePixels(int x, int y);

  void getSize(uint &width, uint &height) const;
  Projection getProjectionType() const;

public:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  glm::vec3 center;

protected:
  float fovY;
  float near;
  float far;
  uint screenWidth;
  uint screenHeight;
  Projection projType;

private:
  void computeProjection();
  void computeView();
};
