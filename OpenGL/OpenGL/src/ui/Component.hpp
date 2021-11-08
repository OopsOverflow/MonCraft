#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "style/Style.hpp"
#include "ui/Key.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Specification.hpp"

class Event;

namespace ui {

enum class Anchor { BEGIN, END, CENTER };

class Component {

protected:
  Component();
  void initialize();

public:
  virtual ~Component();
  Component(Component&) = delete;
  Component& operator=(Component&) = delete;

  virtual void draw();

  void makeActive();

  static const spec_t SIZE;
  static const spec_t POSITION;
  static const spec_t PADDING;
  static const spec_t ANCHOR_X;
  static const spec_t ANCHOR_Y;


  //// STYLING ////
public:
  /**
  * Set a style property only for itself
  */
  virtual void setProperty(prop_t prop);

  /**
   * Shorthand, @see setProperty
   */
  template<typename T>
  void setProp(spec_t spec, T val) {
    setProperty(make_prop(spec, val));
  }

  /**
   * Get the style property
   */
  virtual prop_t getProperty(spec_t spec) const;

protected:
  void setStyle(prop_t prop);

  template<typename T>
  void setStyle(spec_t spec, T val) {
    setStyle(make_prop(spec, val));
  }

  prop_t getStyle(spec_t spec) const;

  template<typename T>
  T getStyle(spec_t spec) const {
    return getStyle(spec).value->get<T>();
  }

  virtual style_const_t getDefaultStyle() const;

  void setStylesheet(style_const_t style);
  style_const_t getStylesheet() const;
  style_const_t getOwnStylesheet();

private:
  style_t ownStyle;
  style_const_t style;

  prop_t getStyleRec(spec_t spec) const;
  void applyStyleRec(style_const_t style);


  //// PROPERTIES ////
public:
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

//// PROTECTED STUFF ////
protected:
  Component* parent;
  std::vector<Component*> children;

  void queueDraw();
  void recompute();
  void handleEvents(std::vector<Event> const& events);
  glm::ivec2 toRelative(glm::ivec2 absPos) const;

  bool isHover();
  bool isPressed();
  bool isActive();
  void unfocus();

  virtual void onMouseIn(glm::ivec2 pos);
  virtual void onMouseOut(glm::ivec2 pos);
  virtual bool onMouseMove(glm::ivec2 pos);
  virtual bool onMousePressed(glm::ivec2 pos);
  virtual bool onMouseReleased(glm::ivec2 pos);
  virtual bool onActivate();
  virtual void onDeactivated();
  virtual void onKeyPressed(Key k);
  virtual void onKeyReleased(Key k);

//// PRIVATE STUFF ////
private:
  bool drawQueued;
  bool recomputeQueued;
  std::vector<std::unique_ptr<Component>> owned;

  glm::ivec2 computedSize;
  glm::ivec2 computedOrigin;

  bool hover;
  bool pressed;

  static Component* activeWidget;

  void queueRecompute(bool propagate = true);
  void computeSize();
  void computeOrigin();
  bool handleEvent(Event const& evt);
  bool bubbleEvent(Event const& evt);
  void filterEvent(Event const& evt);
  bool overlaps(glm::ivec2 point) const;
};

}; // namespace ui
