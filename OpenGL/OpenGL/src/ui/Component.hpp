#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace ui {

enum class Anchor { BEGIN, END, CENTER };

class Component {

public:
  Component(Component* parent);
  virtual ~Component();

  virtual void draw();

  // avoid unecessary draw calls
  void queueDraw();

  void setSize(glm::ivec2 size);
  glm::ivec2 getSize() const;
  glm::ivec2 getAbsoluteSize() const;

  glm::ivec2 getOrigin() const;
  glm::ivec2 getAbsoluteOrigin() const;

  void setPosition(glm::ivec2 position);
  glm::ivec2 getPosition() const;

  void setAnchorX(Anchor anchor);
  Anchor getAnchorX() const;

  void setAnchorY(Anchor anchor);
  Anchor getAnchorY() const;

protected:
  void addChild(Component* child);
  void removeChild(Component* child);
  void recompute();

  bool drawQueued;
  bool recomputeQueued;
  Component* parent;
  std::vector<Component*> children;

private:
  void queueRecompute(bool propagate = true);
  void computeSize();
  void computeOrigin();

  glm::ivec2 size;
  glm::ivec2 absoluteOrigin;
  glm::ivec2 absoluteSize;
  glm::ivec2 computedOrigin;
  glm::ivec2 computedSize;
  glm::ivec2 position;
  Anchor anchorX;
  Anchor anchorY;
};

}; // namespace ui
