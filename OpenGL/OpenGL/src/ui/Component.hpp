#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "style/Style.hpp"
#include "Event.hpp"
#include "Key.hpp"

namespace ui {

enum class Anchor { BEGIN, END, CENTER };

class Component {

public:
  Component();
  virtual ~Component();
  Component(Component&) = delete;
  Component& operator=(Component&) = delete;

  virtual void draw();

  static const spec_t SIZE;
  static const spec_t POSITION;
  static const spec_t PADDING;
  static const spec_t ANCHOR_X;
  static const spec_t ANCHOR_Y;
  virtual void setStyle(prop_t const& prop);
  virtual prop_t getStyle(spec_t spec) const;
  virtual style_const_t getDefaultStyle() const;

  // avoid unecessary draw calls
  void queueDraw();

  void setSize(glm::ivec2 size);
  glm::ivec2 getSize() const;
  glm::ivec2 getAbsoluteSize() const;
  glm::ivec2 getMaxSize() const;

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

  void keyPress(Key k);
  void keyRelease(Key k);

  void add(Component* child);
  void add(std::unique_ptr<Component> child);
  void remove(Component* child);

protected:
  void recompute();

  bool drawQueued;
  bool recomputeQueued;
  Component* parent;
  std::vector<Component*> children;
  std::vector<std::unique_ptr<Component>> owned;

  void handleEvents(std::vector<Event> const& events);
  virtual void onMouseIn(glm::ivec2 pos);
  virtual void onMouseOut(glm::ivec2 pos);
  virtual bool onMouseMove(glm::ivec2 pos);
  virtual bool onMousePressed(glm::ivec2 pos);
  virtual bool onMouseReleased(glm::ivec2 pos);
  virtual bool onActivate();
  virtual void onDeactivated();
  virtual void onKeyPressed(Key k);
  virtual void onKeyReleased(Key k);

  bool isHover();
  bool isPressed();
  bool isActive();
  void unfocus();

private:
  void queueRecompute(bool propagate = true);
  void computeSize();
  void computeOrigin();
  bool handleEvent(Event const& evt);
  bool bubbleEvent(Event const& evt);
  void filterEvent(Event const& evt);
  bool overlaps(glm::ivec2 point) const;
  void makeActive();

  glm::ivec2 size;
  glm::ivec2 absoluteSize;
  glm::ivec2 computedSize;
  glm::ivec2 computedOrigin;
  glm::ivec2 position;
  glm::ivec2 padding;
  Anchor anchorX;
  Anchor anchorY;

  bool hover;
  bool pressed;
  static Component* activeWidget;
};

}; // namespace ui
