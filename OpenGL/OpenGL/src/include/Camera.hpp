#pragma once

#include <glm/glm.hpp>

enum Projection { PROJECTION_ORTHOGRAPHIC,
                  PROJECTION_PERSPECTIVE };

class Camera {
public:
  Camera(unsigned int width, unsigned int height, const glm::vec3 &position,
         const glm::vec3 &center, Projection proj = PROJECTION_PERSPECTIVE);

  void activate();

  void setSize(unsigned int width, unsigned int height);
  void setProjectionType(Projection projType);
  void setPosition(const glm::vec3 &position);
  void setLookAt(const glm::vec3 &lookAt);

  void zoom(float factor);

  void translate(const glm::vec3 &direction, bool localSpace = false);
  void translatePixels(int x, int y);

  void rotate(const glm::vec3 &rotation, bool localSpace = false);
  void rotatePixels(int x, int y);

  void getSize(unsigned int &width, unsigned int &height) const;
  Projection getProjectionType() const;

public:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  glm::vec3 center;

protected:
  float fovY;
  float near_;
  float far_;
  unsigned int screenWidth;
  unsigned int screenHeight;
  Projection projType;

private:
  void computeProjection();
  void computeView();
};
