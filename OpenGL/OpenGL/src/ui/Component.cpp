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
    hover(false), pressed(false),
    ownStyle(std::make_shared<Style>())
{}

void Component::initialize()
{}

Component::~Component() {
  if(parent) parent->remove(this);
  if(activeWidget == this) activeWidget = nullptr;
  for(Component* child : children) {
    child->parent = nullptr;
  }
}

void Component::setStyle(prop_t prop) {
  ownStyle->set(prop);
}

std::shared_ptr<const AbstractValue> Component::getStyle(spec_t spec) const {
  auto res = ownStyle->get(spec);
  if(res.value == nullptr) res = getDefaultStyle()->get(spec);
  if(res.value == nullptr) {
    std::cout << "[WARN] cannot get style property "
              << Specification::get(spec).name << std::endl;
    // throw std::runtime_error("cannot get style property");
  }
  return res.value;
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
  else {
    // std::cout << "[WARN] unsupported style property: '"
    //           << Specification::get(prop.spec).name
    //           << "'"
    //           << std::endl;
  }
}

prop_t Component::getProperty(spec_t spec) const {
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
    Component::SIZE, ivec2(0.f),
    Component::POSITION, ivec2(0.f),
    Component::PADDING, ivec2(0.f),
    Component::ANCHOR_X, Anchor::BEGIN,
    Component::ANCHOR_Y, Anchor::BEGIN
  );

  return style;
}

style_const_t Component::getOwnStyle() const {
  return ownStyle;
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

ivec2 Component::getOrigin() const {
  if(!parent) return getPosition();
  ivec2 orig = getPosition();
  ivec2 size = computedSize;
  ivec2 parentSize = parent->computedSize - parent->getPadding() * 2;

  if(getAnchorX() == Anchor::END)         orig.x +=  parentSize.x - size.x;
  else if(getAnchorX() == Anchor::CENTER) orig.x += (parentSize.x - size.x) / 2.f;
  if(getAnchorY() == Anchor::END)         orig.y +=  parentSize.y - size.y;
  else if(getAnchorY() == Anchor::CENTER) orig.y += (parentSize.y - size.y) / 2.f;

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
  this->size = size;
  queueRecompute(true); // needs to recompute children (size changed)
}

ivec2 Component::getSize() const {
  return size;
}

void Component::setPosition(ivec2 position) {
  if(position == getPosition()) return;
  this->position = position;
  queueRecompute(false); // no need to recompute children
}

ivec2 Component::getPosition() const {
  return position;
}

void Component::setPadding(ivec2 padding) {
  if(padding == getPadding()) return;
  this->padding = padding;
  queueRecompute(true);
}

ivec2 Component::getPadding() const {
  return padding;
}

void Component::setAnchorX(Anchor anchor) {
  if(anchor == getAnchorX()) return;
  this->anchorX = anchor;
  queueRecompute(false); // no need to recompute children
}

Anchor Component::getAnchorX() const {
  return anchorX;
}

void Component::setAnchorY(Anchor anchor) {
  if(anchor == getAnchorY()) return;
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

void Component::onMouseIn(ivec2 pos) { }
void Component::onMouseOut(ivec2 pos) { }
bool Component::onMouseMove(ivec2 pos) { return false; }
bool Component::onMousePressed(ivec2 pos) { return false; }
bool Component::onMouseReleased(ivec2 pos) { return false; }
bool Component::onActivate() { return false; }
void Component::onDeactivated() { }
void Component::onKeyPressed(Key k) { }
void Component::onKeyReleased(Key k) { }
