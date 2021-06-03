#include "Camera.hpp"
#include "../gl/Shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(glm::ivec2 size, const glm::vec3 &position,
               const glm::vec3 &center, Projection projType)
 : view(1.f), projection(1.f),
   position(position), center(center),
   near_(0.1f), far_(200.f), fovY(45.f),
   size(size), projType(projType)
{
  computeView();
  computeProjection();
}

void Camera::activate() {
  glViewport(0, 0, size.x, size.y);
  glm::mat4 normal = glm::transpose(glm::inverse(view));
  Shader *shader = Shader::getActive();
  if (shader) {
    glUniformMatrix4fv(MATRIX_VIEW, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(MATRIX_NORMAL, 1, GL_FALSE, glm::value_ptr(normal));
    glUniformMatrix4fv(MATRIX_PROJECTION, 1, GL_FALSE, glm::value_ptr(projection));
    glm::vec3 c = view * glm::vec4(center, 1.f);
    glUniform3f(CAMERA_CENTER, c.x, c.y, c.z);
  } else {
    std::cout << "error: camera activated but no shader bound" << std::endl;
  }
}

void Camera::setPosition(const glm::vec3 &newPos) {
  view = glm::translate(view, -(newPos - position));
  position = newPos;
}

void Camera::setLookAt(const glm::vec3 &position, const glm::vec3 &center) {
  this->position = position;
  this->center = center;
  computeView();
  computeProjection();
}


void Camera::translate(const glm::vec3 &translation, bool localSpace) {
  glm::mat4 trans = glm::translate(glm::mat4(1.f), translation);
  glm::mat4 invTrans = glm::translate(glm::mat4(1.f), -translation);

  if (localSpace) {
    position = glm::inverse(view) * trans * view * glm::vec4(position, 1.f);
    center = glm::inverse(view) * trans * view * glm::vec4(center, 1.f);
    view = invTrans * view;
  }
  else {
    view = view * invTrans;
    position += translation;
    center += translation;
  }
}

void Camera::rotate(const glm::vec3 &rotation, bool localSpace) {
  glm::mat4 rot(1.f);
  rot = glm::rotate(rot, glm::radians(rotation.x), {1, 0, 0});
  rot = glm::rotate(rot, glm::radians(rotation.y), {0, 1, 0});
  rot = glm::rotate(rot, glm::radians(rotation.z), {0, 0, 1});

  if (localSpace) {
    center = glm::inverse(view) * rot * view * glm::vec4(center, 1.f);
    view = glm::inverse(rot) * view;
  } else {
    glm::mat4 trans = glm::translate(glm::mat4(1.f), center);
    position = trans * rot * glm::inverse(trans) * glm::vec4(position, 1.f);
    view = view * glm::inverse(trans) * glm::inverse(rot) * trans;
  }

  // rotation can accumulate imperfections - better to recalculate.
  computeView();
}

void Camera::setSize(glm::ivec2 size) {
  this->size = size;
  computeProjection();
}

void Camera::setProjectionType(Projection projType) {
  this->projType = projType;
  computeProjection();
}

void Camera::setProjectionType(Projection projType, float box[6]) {
  this->projType = projType;
  computeProjection(box);
}

void Camera::translatePixels(int x, int y) {
  glm::vec3 translation((float)x, (float)-y, 0.f);

  float aspect = (float)size.x / (float)size.y;
  float localHeight =
      2.f * glm::length(center - position) * tan(glm::radians(fovY / 2.f));
  float localWidth = localHeight * aspect;

  translation.x *= localWidth / (float)size.x;
  translation.y *= localHeight / (float)size.y;

  translate(translation, true);
}

void Camera::rotatePixels(int x, int y, bool localSpace) {
  // in the turnTable rotation style we rotate around y axis in global space
  // and around x axis in local space.

  float rotX, rotY;

  // can rotate by maxRotation degrees around x axis while translating from 0
  // to size.x (resp. y axis & size.y).
  float maxRotation = 360.f;

  rotY = x * maxRotation / (float)size.x;
  rotX = y * maxRotation / (float)size.y;

  if(localSpace) {
    rotate({rotX, rotY, 0.f}, localSpace);
  } else {
    rotate({0.f, rotY, 0.f});
    // rotAxis should be normalized already (translations and rotations)
    glm::vec3 rotAxis = glm::inverse(view) * glm::vec4(1.f, 0.f, 0.f, 0.f);
    rotate(rotX * rotAxis);
  }
}

void Camera::setFovY(float fovY) {
  this->fovY = fovY;
}

// ----------- getters -----------

float Camera::getFovY() const {
   return fovY;
}

// see https://en.wikipedia.org/wiki/Field_of_view_in_video_games#Field_of_view_calculations
float Camera::getFovX() const {
  return glm::degrees(2 * atan(tan(glm::radians(fovY) * 0.5) * size.x / size.y));
}

glm::ivec2 Camera::getSize() const {
  return size;
}

Projection Camera::getProjectionType() const { return projType; }

// ----------- private -----------

void Camera::computeView() {
  glm::vec3 up(0.f, 1.f, 0.f);
  glm::vec3 cameraUp = view * glm::vec4(up, 0.0);
  if (glm::dot(up, cameraUp) < 0) {
    up = -up;
  }
  view = glm::lookAt(position, center, up);
}

void Camera::computeProjection(float box[6]) {
  float aspect = (float)size.x / (float)size.y;

  if (projType == Projection::PROJECTION_ORTHOGRAPHIC) {
    // kind of perspective division... To switch between persp & ortho.
    tanFovY = tan(glm::radians(getFovY()) * 0.5);
    tanFovX = tan(glm::radians(getFovX()) * 0.5);
    float y = glm::length(center - position) * tanFovY;
    float x = y * aspect;
    projection = glm::ortho(-x, x, -y, y, 0.f, 1000.f);
  }

  else if (projType == Projection::PROJECTION_PERSPECTIVE) {
    projection = glm::perspective(glm::radians(fovY), aspect, near_, far_);
    tanFovY = tan(glm::radians(getFovY()) * 0.5);
    tanFovX = tan(glm::radians(getFovX()) * 0.5);

  }
  else {
    projection = glm::ortho(box[0], box[1], box[2], box[3], box[4], box[5]);
    custumProjBox[0] = box[0];
    custumProjBox[1] = box[1];
    custumProjBox[2] = box[2];
    custumProjBox[3] = box[3];
    custumProjBox[4] = box[4];
    custumProjBox[5] = box[5];

  }
}

void Camera::computeProjection() {
    float defaults[6] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    computeProjection(defaults);
}

std::vector<glm::vec3> Camera::getBoxCorners(Frustum frustum) const {
    float z1, z2;
    float range = far_ - near_;

    switch (frustum)
    {
    case Frustum::ALL:
        z1 = -near_;
        z2 = -far_;
        break;
    case Frustum::NEAR:
        z1 = -near_;
        z2 = -near_ - range / 8.0f;
        break;
    case Frustum::MEDIUM:
        z1 = -near_ - range / 8.0f;
        z2 = -near_ - 3.0f * range / 8.0f;
        break;
    case Frustum::FAR:
        z1 = -near_ - 3.0f * range / 8.0f;
        z2 = -far_;
        break;
    default:
        break;
    }

    float y1;
    float y2;
    float x1;
    float x2;

    if (projType == Projection::PROJECTION_PERSPECTIVE) {
        y1 = z1 * tanFovY;
        y2 = z2 * tanFovY;

        x1 = z1 * tanFovX;
        x2 = z2 * tanFovX;

    }
    else if(projType == Projection::PROJECTION_ORTHOGRAPHIC) {
        float aspect = (float)size.x / (float)size.y;
        float y = glm::length(center - position) * tanFovY;
        float x = y * aspect;

        z1 = 0.0f;
        z2 = 1000.0f;

        y1 = y;
        y2 = y;

        x1 = x;
        x2 = x;
    }
    else {
      throw std::runtime_error("unimplemented");
    }

    glm::mat4 invertView = glm::inverse(view);

    std::vector<glm::vec3> pos;
    //near
    pos.push_back(glm::vec3(invertView * glm::vec4(x1, y1, z1, 1.0f))); //top, right
    pos.push_back(glm::vec3(invertView * glm::vec4(x1, -y1, z1, 1.0f))); //bot, right
    pos.push_back(glm::vec3(invertView * glm::vec4(-x1, y1, z1, 1.0f))); //top, left
    pos.push_back(glm::vec3(invertView * glm::vec4(-x1, -y1, z1, 1.0f))); //bot, left

    //far
    pos.push_back(glm::vec3(invertView * glm::vec4(x2, y2, z2, 1.0f)));
    pos.push_back(glm::vec3(invertView * glm::vec4(x2, -y2, z2, 1.0f)));
    pos.push_back(glm::vec3(invertView * glm::vec4(-x2, y2, z2, 1.0f)));
    pos.push_back(glm::vec3(invertView * glm::vec4(-x2, -y2, z2, 1.0f)));

    return pos;
}



bool Camera::chunkInView(glm::vec3 posCamSpace, float tolerance) const {
    if (projType == Projection::PROJECTION_PERSPECTIVE) {
        bool inFrustum = true;
        auto farChunkZ = posCamSpace.z - tolerance;
        auto frustumXDelta = farChunkZ * tanFovX;
        auto frustumYDelta = farChunkZ * tanFovY;
        inFrustum &= posCamSpace.z - tolerance < -near_;
        inFrustum &= posCamSpace.z + tolerance > -far_;
        inFrustum &= posCamSpace.x - tolerance < -frustumXDelta;
        inFrustum &= posCamSpace.x + tolerance > frustumXDelta;
        inFrustum &= posCamSpace.y - tolerance < -frustumYDelta;
        inFrustum &= posCamSpace.y + tolerance > frustumYDelta;

        return inFrustum;
    }
    else if (projType == Projection::CUSTOM_PROJECTION) {
        bool inFrustum = true;

        inFrustum &= posCamSpace.x - tolerance > custumProjBox[0];
        inFrustum &= posCamSpace.x + tolerance < custumProjBox[1];
        inFrustum &= posCamSpace.y - tolerance > custumProjBox[2];
        inFrustum &= posCamSpace.y + tolerance < custumProjBox[3];
        inFrustum &= posCamSpace.z - tolerance > custumProjBox[4];
        inFrustum &= posCamSpace.z + tolerance < custumProjBox[5];

        return 1; //TODO we don't manage to make it work
    }
    bool inFrustum = true;
    float aspect = (float)size.x / (float)size.y;
    float y = glm::length(center - position) * tanFovY;
    float x = y * aspect;

    inFrustum &= posCamSpace.x - tolerance < -x;
    inFrustum &= posCamSpace.x + tolerance > x;
    inFrustum &= posCamSpace.y - tolerance < -y;
    inFrustum &= posCamSpace.y + tolerance > y;
    inFrustum &= posCamSpace.z - tolerance < 0.0f;
    inFrustum &= posCamSpace.z + tolerance > 1000.0f;

    return inFrustum;


}
