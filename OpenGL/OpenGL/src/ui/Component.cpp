#include <algorithm>
#include "Component.hpp"

using namespace ui;
using namespace glm;

Component* Component::activeWidget = nullptr;

MAKE_TYPE(Anchor);
const spec_t Component::SIZE     = MAKE_SPEC("Component::size", ivec2);
const spec_t Component::POSITION = MAKE_SPEC("Component::position", ivec2);
const spec_t Component::PADDING  = MAKE_SPEC("Component::padding", ivec2);
const spec_t Component::ANCHOR_X = MAKE_SPEC("Component::anchorX", Anchor);
const spec_t Component::ANCHOR_Y = MAKE_SPEC("Component::anchorY", Anchor);

Component::Component()
  : drawQueued(true), recomputeQueued(true),
    parent(nullptr),
    size(0), absoluteSize(0), computedSize(0), computedOrigin(0),
    position(0), padding(0),
    anchorX(Anchor::BEGIN), anchorY(Anchor::BEGIN),
    hover(false), pressed(false)
{
  Component::getDefaultStyle()->apply(this);
}

Component::~Component() {
  if(parent) parent->remove(this);
  if(activeWidget == this) activeWidget = nullptr;
  for(Component* child : children) {
    child->parent = nullptr;
  }
}

void Component::setStyle(prop_t const& prop) {
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
  else {
    std::cout << "[WARN] unsupported style property: '"
              << Specification::get(prop.spec).name
              << "'"
              << std::endl;
  }
}

prop_t Component::getStyle(spec_t spec) const {
  if(spec == Component::SIZE) {
    return make_property(spec, getSize());
  }
  else if(spec == Component::POSITION) {
    return make_property(spec, getPosition());
  }
  else if(spec == Component::PADDING) {
    return make_property(spec, getPadding());
  }
  else if(spec == Component::ANCHOR_X) {
    return make_property(spec, getAnchorX());
  }
  else if(spec == Component::ANCHOR_Y) {
    return make_property(spec, getAnchorY());
  }
  else {
    throw StyleError(
      "unsupported style property: " +
      Specification::get(spec).name +
      "'"
    );
  }
}

style_const_t Component::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    nullptr, // parent
    make_property(Component::SIZE, ivec2(0.f)),
    make_property(Component::POSITION, ivec2(0.f)),
    make_property(Component::PADDING, ivec2(0.f)),
    make_property(Component::ANCHOR_X, Anchor::BEGIN),
    make_property(Component::ANCHOR_Y, Anchor::BEGIN)
  );

  return style;
}

void Component::draw() {
  if(!drawQueued) return;
  for(Component* child : children) child->draw();
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
  queueRecompute(true);
  if(parent) parent->queueRecompute();
}

void Component::remove(Component* child) {
  if(!child->parent || child->parent != this) {
    throw std::runtime_error("element is not a child");
  }
  children.erase(std::remove(children.begin(), children.end(), child));
  child->parent = nullptr;
}

ivec2 Component::getOrigin() const {
  if(!parent) return position;
  ivec2 orig = position;
  ivec2 size = computedSize;
  ivec2 parentSize = parent->computedSize - parent->padding * 2;

  if(anchorX == Anchor::END)         orig.x +=  parentSize.x - size.x;
  else if(anchorX == Anchor::CENTER) orig.x += (parentSize.x - size.x) / 2.f;
  if(anchorY == Anchor::END)         orig.y +=  parentSize.y - size.y;
  else if(anchorY == Anchor::CENTER) orig.y += (parentSize.y - size.y) / 2.f;

  return orig;
}

ivec2 Component::getAbsoluteOrigin() const {
  if(!parent) return computedOrigin;
  return parent->getAbsoluteOrigin() + parent->padding + computedOrigin;
}

ivec2 Component::getAbsoluteSize() const {
  return computedSize;
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
    ivec2 newCompSize = size + 2 * padding;

    for(Component* child : children) {
      newCompSize = max(newCompSize, abs(child->position) + child->computedSize + 2 * padding);
    }

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
      for(auto child : children)
        child->bubbleEvent(evt);
      onMouseOut(evt.getPosition());
    }
    return false;
  }

  else {
    for(auto it = children.rbegin(); it != children.rend(); it++) {
      Component* child = *it;
      if(child->bubbleEvent(evt)) return true;
    }
    if(evt.getType() == Event::Type::PRESS) makeActive();
    filterEvent(evt);
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
  ivec2 pos = evt.getPosition();
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

void Component::setSize(glm::ivec2 size) {
  if(size == this->size) return;
  this->size = size;
  queueRecompute(true); // needs to recompute children (size changed)
}

ivec2 Component::getSize() const {
  return size;
}

void Component::setPosition(glm::ivec2 position) {
  if(position == this->position) return;
  this->position = position;
  queueRecompute(false); // no need to recompute children
}

glm::ivec2 Component::getPosition() const {
  return position;
}

void Component::setPadding(glm::ivec2 padding) {
  if(padding == this->padding) return;
  this->padding = padding;
  queueRecompute(true);
}

glm::ivec2 Component::getPadding() const {
  return padding;
}

void Component::setAnchorX(Anchor anchor) {
  if(anchor == this->anchorX) return;
  this->anchorX = anchor;
  queueRecompute(false); // no need to recompute children
}

Anchor Component::getAnchorX() const {
  return anchorX;
}

void Component::setAnchorY(Anchor anchor) {
  if(anchor == this->anchorY) return;
  this->anchorY = anchor;
  queueRecompute(false); // no need to recompute children
}

Anchor Component::getAnchorY() const {
  return anchorY;
}

void Component::keyPress(Key k) {
  if(activeWidget) activeWidget->onKeyPressed(k);
}

void Component::keyRelease(Key k) {
  if(activeWidget) activeWidget->onKeyReleased(k);
}

// default event handlers

void Component::onMouseIn(glm::ivec2 pos) { }
void Component::onMouseOut(glm::ivec2 pos) { }
bool Component::onMouseMove(glm::ivec2 pos) { return false; }
bool Component::onMousePressed(glm::ivec2 pos) { return false; }
bool Component::onMouseReleased(glm::ivec2 pos) { return false; }
bool Component::onActivate() { return false; }
void Component::onDeactivated() { }
void Component::onKeyPressed(Key k) { }
void Component::onKeyReleased(Key k) { }
