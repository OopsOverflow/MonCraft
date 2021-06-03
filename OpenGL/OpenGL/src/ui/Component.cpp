#include <algorithm>
#include "Component.hpp"

using namespace ui;
using namespace glm;

MAKE_TYPE(Anchor);
const spec_t Component::SIZE     = MAKE_SPEC("Component::size", ivec2);
const spec_t Component::POSITION = MAKE_SPEC("Component::position", ivec2);
const spec_t Component::PADDING  = MAKE_SPEC("Component::padding", ivec2);
const spec_t Component::ANCHOR_X = MAKE_SPEC("Component::anchorX", Anchor);
const spec_t Component::ANCHOR_Y = MAKE_SPEC("Component::anchorY", Anchor);

Component::Component(Component* parent)
  : drawQueued(true), recomputeQueued(true),
    parent(parent), position(0.f),
    anchorX(Anchor::BEGIN), anchorY(Anchor::BEGIN)
{
  if(parent) parent->addChild(this);
}

Component::~Component() {
  if(parent) parent->removeChild(this);
}

void Component::setStyle(prop_t prop) {
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

void Component::addChild(Component* child) {
  children.push_back(child);
  queueRecompute(true);
  if(parent) parent->queueRecompute();
}

void Component::removeChild(Component* child) {
  children.erase(std::remove(children.begin(), children.end(), child));
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

void Component::setSize(glm::ivec2 size) {
  if(size == this->size) return;
  this->size = size;
  queueRecompute(true); // needs to recompute children (size changed)
}

ivec2 Component::getSize() const {
  return size;
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
