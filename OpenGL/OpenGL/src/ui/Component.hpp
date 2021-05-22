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

  virtual glm::ivec2 getSize() const = 0;
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

  bool drawQueued;
  Component* parent;
  std::vector<Component*> children;

private:
  glm::ivec2 position;
  Anchor anchorX;
  Anchor anchorY;
};

}; // namespace ui
