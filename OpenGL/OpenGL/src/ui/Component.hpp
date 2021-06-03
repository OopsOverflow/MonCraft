#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "style/Style.hpp"

namespace ui {

enum class Anchor { BEGIN, END, CENTER };

class Component {

public:
  Component(Component* parent);
  virtual ~Component();
  Component(Component&) = delete;
  Component& operator=(Component&) = delete;

  virtual void draw();

  static const spec_t SIZE;
  static const spec_t POSITION;
  static const spec_t PADDING;
  static const spec_t ANCHOR_X;
  static const spec_t ANCHOR_Y;
  virtual void setStyle(prop_t prop);

  // avoid unecessary draw calls
  void queueDraw();

  void setSize(glm::ivec2 size);
  glm::ivec2 getSize() const;
  glm::ivec2 getAbsoluteSize() const;

  glm::ivec2 getOrigin() const;
  glm::ivec2 getAbsoluteOrigin() const;

  void setPosition(glm::ivec2 position);
  glm::ivec2 getPosition() const;

  void setPadding(glm::ivec2 padding);
  glm::ivec2 getPadding() const;

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
  glm::ivec2 absoluteSize;
  glm::ivec2 computedSize;
  glm::ivec2 computedOrigin;
  glm::ivec2 position;
  glm::ivec2 padding;
  Anchor anchorX;
  Anchor anchorY;
};

}; // namespace ui
