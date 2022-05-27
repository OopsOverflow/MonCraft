#include <glm/glm.hpp>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include "ui/Component.hpp"
#include "ui/Event.hpp"
#include "ui/Key.hpp"
#include "ui/style/Style.hpp"
#include "ui/style/StyleError.hpp"
#include "ui/style/Type.hpp"
#include "ui/style/Value.hpp"

using namespace ui;
using namespace glm;

Component* Component::activeWidget = nullptr;

MAKE_TYPE(Anchor);
const spec_t Component::SIZE     = MAKE_SPEC("Component::size", ivec2);
const spec_t Component::POSITION = MAKE_SPEC("Component::position", ivec2);
const spec_t Component::PADDING  = MAKE_SPEC("Component::padding", ivec2);
const spec_t Component::ANCHOR_X = MAKE_SPEC("Component::anchorX", Anchor);
const spec_t Component::ANCHOR_Y = MAKE_SPEC("Component::anchorY", Anchor);
const spec_t Component::HIDDEN = MAKE_SPEC_INHERIT("Component::hidden", bool);

Component::Component()
  : ownStyle(std::make_shared<Style>()), style(ownStyle), parent(nullptr),
    drawQueued(true), recomputeQueued(true),
    computedSize(0), computedOrigin(0),
    hover(false), pressed(false)
{}


void Component::applyStyleRec(style_const_t style) {
  if(auto parent = style->getParent()) applyStyleRec(parent);
  for(auto pair : *style) setProperty(pair.second);
}

void Component::initialize() {
  setStylesheet(getOwnStylesheet());
}

Component::~Component() {
  if(parent) parent->remove(this);
  if(activeWidget == this) activeWidget = nullptr;
  for(Component* child : children) {
    child->parent = nullptr;
  }
}


void Component::setStylesheet(style_const_t style) {
  this->style = style;
}

style_const_t Component::getStylesheet() const {
  return style;
}

style_const_t Component::getOwnStylesheet() {
  return ownStyle;
}

void Component::setStyle(prop_t prop) {
  ownStyle->set(prop);
}

prop_t Component::getStyleRec(spec_t spec) const {
  prop_t res{ spec, nullptr };
  if(style) res = style->get(spec);

  if(parent && res.value == nullptr && Spec::get(spec).inherit) {
    res = parent->getStyleRec(spec);
  }

  return res;
}

prop_t Component::getStyle(spec_t spec) const {
  prop_t res = getStyleRec(spec);
  if(res.value == nullptr) res = getDefaultStyle()->get(spec);
  if(res.value == nullptr) {
    throw std::runtime_error("cannot get style property");
  }
  return res;
}

void Component::setProperty(prop_t prop) {
  if(prop.spec == Component::SIZE) {
    setSize(prop.value->get<ivec2>());
  }
  else if(prop.spec == Component::POSITION) {
    setPosition(prop.value->get<ivec2>());
  }
  else if(prop.spec == Component::PADDING) {
    setPadding(prop.value->get<ivec2>());
  }
  else if(prop.spec == Component::ANCHOR_X) {
    setAnchorX(prop.value->get<Anchor>());
  }
  else if(prop.spec == Component::ANCHOR_Y) {
    setAnchorY(prop.value->get<Anchor>());
  }
  else if(prop.spec == Component::HIDDEN) {
    setHidden(prop.value->get<bool>());
  }
  else {
    setStyle(prop);
    // std::cout << "[WARN] unsupported style property: '"
    //           << Spec::get(prop.spec).name
    //           << "'"
    //           << std::endl;
  }
}

prop_t Component::getProperty(spec_t spec) const {
  if(spec == Component::SIZE) {
    return make_prop(spec, getSize());
  }
  else if(spec == Component::POSITION) {
    return make_prop(spec, getPosition());
  }
  else if(spec == Component::PADDING) {
    return make_prop(spec, getPadding());
  }
  else if(spec == Component::ANCHOR_X) {
    return make_prop(spec, getAnchorX());
  }
  else if(spec == Component::ANCHOR_Y) {
    return make_prop(spec, getAnchorY());
  }
  else if(spec == Component::HIDDEN) {
    return make_prop(spec, getHidden());
  }
  else {
    throw StyleError(
      "unsupported style property: " +
      Spec::get(spec).name +
      "'"
    );
  }
}

style_const_t Component::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    nullptr, // parent
    Component::SIZE, ivec2(0),
    Component::POSITION, ivec2(0),
    Component::PADDING, ivec2(0),
    Component::ANCHOR_X, Anchor::BEGIN,
    Component::ANCHOR_Y, Anchor::BEGIN,
    Component::HIDDEN, false
  );

  return style;
}

void Component::draw() {
  queueRecompute(false); // TODO wow this is a hack
  if(!drawQueued) return;
  for(Component* child : children) {
    if(!child->getHidden())
      child->draw();
  }
  drawQueued = false;
}

void Component::queueDraw() {
  drawQueued = true;
  for(Component* child : children) child->queueDraw();
}

void Component::queueRecompute(bool propagate) {
  recomputeQueued = true;
  queueDraw();
  if(propagate) for(Component* child : children) child->queueRecompute(true);
}

void Component::add(Component* child) {
  if(child->parent != nullptr) {
    throw std::runtime_error("child already has parent");
  }

  child->parent = this;
  children.push_back(child);

  // TODO: style

  queueRecompute(true);
  if(parent) parent->queueRecompute();
}

void Component::add(std::unique_ptr<Component> child) {
  owned.push_back(std::move(child));
  add(owned.back().get());
}

void Component::remove(Component* child) {
  if(!child->parent || child->parent != this) {
    throw std::runtime_error("element is not a child");
  }
  children.erase(std::remove(children.begin(), children.end(), child));
  child->parent = nullptr;
}

std::vector<Component*> Component::getChildren() const {
  return children;
}

ivec2 Component::getOrigin() const {
  if(!parent) return getPosition();
  ivec2 orig = getPosition();
  ivec2 size = computedSize;
  ivec2 parentSize = parent->computedSize - parent->getPadding() * 2;

  if(getAnchorX() == Anchor::END)         orig.x +=  parentSize.x - size.x;
  else if(getAnchorX() == Anchor::CENTER) orig.x += (parentSize.x - size.x) / 2;
  if(getAnchorY() == Anchor::END)         orig.y +=  parentSize.y - size.y;
  else if(getAnchorY() == Anchor::CENTER) orig.y += (parentSize.y - size.y) / 2;

  return orig;
}

ivec2 Component::getAbsoluteOrigin() const {
  if(!parent) return computedOrigin;
  return parent->getAbsoluteOrigin() + parent->getPadding() + computedOrigin;
}

ivec2 Component::toRelative(ivec2 pos) const {
  if(!parent) return pos;
  return parent->toRelative(pos) - parent->getPadding() - computedOrigin;
}

ivec2 Component::getAbsoluteSize() const {
  return computedSize;
}

ivec2 Component::getMaxSize() const {
  if(!parent) return getSize();
  return parent->getMaxSize() - 2 * parent->getPadding();
}

void Component::recompute() {
  computeSize();
  computeOrigin(); // also resets recomputeQueued
}

void Component::computeOrigin() { // requires size to be properly computed
  for(Component* child : children) child->computeOrigin();

  if(recomputeQueued) {
    ivec2 newOrig = getOrigin();

    for(Component* child : children) {
      newOrig = min(newOrig, newOrig + child->computedOrigin);
    }

    computedOrigin = newOrig;
    recomputeQueued = false; // finished recompute jobs
  }
}

void Component::computeSize() {
  for(Component* child : children) child->computeSize();

  if(recomputeQueued) {
    ivec2 newCompSize = getSize() + 2 * getPadding();

    for(Component* child : children) {
      newCompSize = max(newCompSize, abs(child->getPosition()) + child->computedSize + 2 * getPadding());
    }

    newCompSize = min(newCompSize, getMaxSize());

    if(parent && newCompSize != computedSize) parent->queueRecompute(false);
    computedSize = newCompSize;
  }
}

bool Component::overlaps(ivec2 point) const {
  ivec2 p1 = getAbsoluteOrigin();
  ivec2 p2 = p1 + getAbsoluteSize();
  return
    p1.x <= point.x && point.x <= p2.x &&
    p1.y <= point.y && point.y <= p2.y;
}

// COMBAK: not sure if this works well.
bool Component::bubbleEvent(Event const& evt) { // goes to the bottom
  if(!overlaps(evt.getPosition())) {
    if(hover) {
      hover = false;
      pressed = false;
      for(auto child : children)
        child->bubbleEvent(evt);
      onMouseOut(evt.getPosition());
    }
    return false;
  }

  else {
    bool bubbled = false;
    for(int it = children.size()-1; it >= 0; it--) {
      Component* child = children.at(it);
      bubbled |= child->bubbleEvent(evt);
    }
    if(!bubbled) {
      if(evt.getType() == Event::Type::PRESS) makeActive();
      filterEvent(evt);
    }
    return true;
  }
}

void Component::filterEvent(Event const& evt) { // goes bottom-up
  bool stopPropagation = handleEvent(evt);
  if(parent && !stopPropagation) {
    parent->filterEvent(evt);
  }
}

bool Component::isHover() {
  return hover;
}

bool Component::isPressed() {
  return pressed;
}

bool Component::isActive() {
  return activeWidget == this;
}

void Component::unfocus() {
  if(activeWidget == this) {
    onDeactivated();
    activeWidget = nullptr;
  }
}

bool Component::handleEvent(Event const& evt) {
  ivec2 pos = toRelative(evt.getPosition());
  Event::Type type = evt.getType();
  bool res = false;

  if(!hover) onMouseIn(pos);

  // state is changed after event handling
  hover = true;

  switch(type) {
    case Event::Type::MOVE:
      res = onMouseMove(pos);
      break;
    case Event::Type::PRESS:
      res = onMousePressed(pos);
      break;
    case Event::Type::RELEASE:
      res = onMouseReleased(pos);
      break;
  }

  // state is changed after event handling
  if(type == Event::Type::PRESS) pressed = true;
  else if(type == Event::Type::RELEASE) pressed = false;

  return res;
}

void Component::handleEvents(std::vector<Event> const& events) {
  for(auto const& evt : events)
    bubbleEvent(evt);
}

void Component::makeActive() {
  if(activeWidget == this) return;
  else if(onActivate()) {
    if(activeWidget) activeWidget->onDeactivated();
    activeWidget = this;
  }
  else if(parent) {
    parent->makeActive();
  }
  else if(activeWidget) {
    activeWidget->onDeactivated();
    activeWidget = nullptr;
  }
}

// style setters / getters below

void Component::setSize(ivec2 size) {
  if(size == getSize()) return;
  setStyle(SIZE, size);
  queueRecompute(true); // needs to recompute children (size changed)
}

ivec2 Component::getSize() const {
  return getStyle<ivec2>(SIZE);
}

void Component::setPosition(ivec2 position) {
  if(position == getPosition()) return;
  setStyle(POSITION, position);
  queueRecompute(false); // no need to recompute children
}

ivec2 Component::getPosition() const {
  return getStyle<ivec2>(POSITION);
}

void Component::setPadding(ivec2 padding) {
  if(padding == getPadding()) return;
  setStyle(PADDING, padding);
  queueRecompute(true); // needs to recompute children (size changed)
}

ivec2 Component::getPadding() const {
  return getStyle<ivec2>(PADDING);
}

void Component::setAnchorX(Anchor anchor) {
  if(anchor == getAnchorX()) return;
  setStyle(ANCHOR_X, anchor);
  queueRecompute(false); // no need to recompute children
}

Anchor Component::getAnchorX() const {
  return getStyle<Anchor>(ANCHOR_X);
}

void Component::setAnchorY(Anchor anchor) {
  if(anchor == getAnchorY()) return;
  setStyle(ANCHOR_Y, anchor);
  queueRecompute(false); // no need to recompute children
}

Anchor Component::getAnchorY() const {
  return getStyle<Anchor>(ANCHOR_Y);
}

void Component::setHidden(bool hidden) {
  if(hidden == getHidden()) return;
  setStyle(HIDDEN, hidden);
  queueRecompute(false); // no need to recompute children
}

bool Component::getHidden() const {
  return getStyle<bool>(HIDDEN);
}

void Component::keyPress(Key k) {
  if(activeWidget) activeWidget->onKeyPressed(k);
}

void Component::keyRelease(Key k) {
  if(activeWidget) activeWidget->onKeyReleased(k);
}

// default event handlers

void Component::onMouseIn(ivec2 pos) { }
void Component::onMouseOut(ivec2 pos) { }
bool Component::onMouseMove(ivec2 pos) { return false; }
bool Component::onMousePressed(ivec2 pos) { return false; }
bool Component::onMouseReleased(ivec2 pos) { return false; }
bool Component::onActivate() { return true; }
void Component::onDeactivated() { }
void Component::onKeyPressed(Key k) { }
void Component::onKeyReleased(Key k) { }
