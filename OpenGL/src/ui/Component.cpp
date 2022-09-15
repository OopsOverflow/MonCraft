#include <cassert>
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

Component* Component::activeComponent = nullptr;
Component* Component::hoverComponent = nullptr;

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
  if(activeComponent == this) activeComponent = nullptr;
  if(hoverComponent == this) {
    if(parent) hoverComponent = parent;
    else hoverComponent = nullptr;
  }
  for(std::shared_ptr<Component> child : children) {
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
  for(std::shared_ptr<Component> child : children) {
    if(!child->getHidden())
      child->draw();
  }
  drawQueued = false;
}

void Component::queueDraw() {
  drawQueued = true;
  for(std::shared_ptr<Component> child : children) child->queueDraw();
}

void Component::queueRecompute(bool propagate) {
  recomputeQueued = true;
  queueDraw();
  if(propagate) for(std::shared_ptr<Component> child : children) child->queueRecompute(true);
}

void Component::add(std::shared_ptr<Component> child) {
  if(child->parent != nullptr) {
    throw std::runtime_error("child already has parent");
  }

  child->parent = this;
  children.push_back(child);

  // TODO: style

  queueRecompute(true);
  if(parent) parent->queueRecompute();
}

void Component::remove(Component* child) {
  if(!child->parent || child->parent != this) {
    throw std::runtime_error("element is not a child");
  }
  child->parent = nullptr;
  auto it = std::find_if(children.begin(), children.end(), [&](auto& other) {
    return other.get() == child;
  });
  children.erase(it);
}

std::vector<std::shared_ptr<Component>> Component::getChildren() const {
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
  for(std::shared_ptr<Component> child : children) child->computeOrigin();

  if(recomputeQueued) {
    ivec2 newOrig = getOrigin();

    for(std::shared_ptr<Component> child : children) {
      newOrig = min(newOrig, newOrig + child->computedOrigin);
    }

    computedOrigin = newOrig;
    recomputeQueued = false; // finished recompute jobs
  }
}

void Component::computeSize() {
  for(std::shared_ptr<Component> child : children) child->computeSize();

  if(recomputeQueued) {
    ivec2 newCompSize = getSize() + 2 * getPadding();

    for(std::shared_ptr<Component> child : children) {
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

bool Component::isHover() {
  return hover;
}

bool Component::isPressed() {
  return pressed;
}

bool Component::isActive() {
  return activeComponent == this;
}

void Component::unfocus() {
  if(activeComponent == this) {
    onDeactivated();
    activeComponent = nullptr;
  }
}

Component* Component::findTargetComponent(ivec2 pos) {
  if(overlaps(pos)) {
    auto childrenCopy = children;
    for(int i = (int)childrenCopy.size() - 1; i >= 0; --i) { // children are traversed last to first (last is on top)
      std::shared_ptr<Component> child = childrenCopy[i];
      Component* target = child->findTargetComponent(pos);
      if(target) return target;
    }
    return this;
  }
  else {
    return nullptr;
  }
}

std::vector<Component*> Component::findAncestors() {
  if(!parent) {
    return { this };
  }
  else {
    std::vector<Component*> ancestors = parent->findAncestors();
    ancestors.push_back(this);
    return ancestors;
  }
}

Component* Component::findCommonAncestor(Component* other) {
  auto ancestorsA = findAncestors();
  auto ancestorsB = other->findAncestors();
  int i = 0;
  int count = std::min(ancestorsA.size(), ancestorsB.size());
  while (ancestorsA[i] == ancestorsB[i] && i < count) {
    i++;
  }
  if (i == 0) return nullptr;
  else return ancestorsA[i - 1];
}

Component* Component::findRoot() {
  if(!parent) return this;
  return parent->findRoot();
}

#include "debug/Debug.hpp"

void Component::handleEvent(Event const& evt) {
  Component* target = findTargetComponent(evt.getPosition());
  Component* ancestor;
  if(!target) return;
  
  // un-hover
  if(hoverComponent) {
    ancestor = target->findCommonAncestor(hoverComponent);
    Component* comp = hoverComponent;
    while(comp && comp != ancestor) {
      if(comp->hover)
        comp->onMouseOut(toRelative(evt.getPosition()));
      comp->hover = false;
      comp->pressed = false;
      comp = comp->parent;
    }
  }
  else {
    ancestor = findRoot();
  }

  // hover
  Component* comp = target;
  while(comp) {
    if(!comp->hover)
      comp->onMouseIn(toRelative(evt.getPosition()));
    comp->hover = true;
    comp = comp->parent;
  }
  hoverComponent = target;
  
  // make active
  if(evt.getType() == Event::Type::PRESS) {
    target->makeActive();
  }
  
  // bubble event (child-to-parent)
  bool stop = false;
  comp = target;
  while(!stop && comp) {
    stop = comp->applyEvent(evt);
    comp = comp->parent;
  }
}

bool Component::applyEvent(Event const& evt) {
  ivec2 pos = toRelative(evt.getPosition());
  switch(evt.getType()) {
    case Event::Type::MOVE:
      return onMouseMove(pos);
    case Event::Type::PRESS:
      pressed = true;
      return onMousePressed(pos);
    case Event::Type::RELEASE:
      pressed = false;
      return onMouseReleased(pos);
    default:
      assert(false);
  }
}

void Component::makeActive() {
  if(activeComponent == this) return;
  else if(onActivate()) {
    if(activeComponent) activeComponent->onDeactivated();
    activeComponent = this;
  }
  else if(parent) {
    parent->makeActive();
  }
  else if(activeComponent) {
    activeComponent->onDeactivated();
    activeComponent = nullptr;
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
  if(activeComponent) activeComponent->onKeyPressed(k);
}

void Component::keyRelease(Key k) {
  if(activeComponent) activeComponent->onKeyReleased(k);
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
