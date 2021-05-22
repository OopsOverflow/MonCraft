#include <algorithm>
#include "Component.hpp"

using namespace ui;
using namespace glm;

Component::Component(Component* parent)
  : drawQueued(true), parent(parent), position(0.f),
    anchorX(Anchor::BEGIN), anchorY(Anchor::BEGIN)
{
  if(parent) parent->addChild(this);
}

Component::~Component() {
  if(parent) parent->removeChild(this);
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

void Component::addChild(Component* child) {
  children.push_back(child);
}

void Component::removeChild(Component* child) {
  children.erase(std::remove(children.begin(), children.end(), child));
}

ivec2 Component::getOrigin() const {
  if(!parent) return position;
  ivec2 orig = position;
  ivec2 size = getSize();
  ivec2 parentSize = parent->getSize();
  if(anchorX == Anchor::END)         orig.x += parentSize.x - size.x;
  else if(anchorX == Anchor::CENTER) orig.x += (parentSize.x - size.x) / 2.f;
  if(anchorY == Anchor::END)         orig.y += parentSize.y - size.y;
  else if(anchorY == Anchor::CENTER) orig.y += (parentSize.y - size.y) / 2.f;

  return orig;
}

#include "debug/Debug.hpp"

ivec2 Component::getAbsoluteOrigin() const {
  if(!parent) return position;
  ivec2 orig = parent->getAbsoluteOrigin() + position;
  ivec2 size = getSize();
  ivec2 parentSize = parent->getSize();
  if(anchorX == Anchor::END)         orig.x += parentSize.x - size.x;
  else if(anchorX == Anchor::CENTER) orig.x += (parentSize.x - size.x) / 2.f;
  if(anchorY == Anchor::END)         orig.y += parentSize.y - size.y;
  else if(anchorY == Anchor::CENTER) orig.y += (parentSize.y - size.y) / 2.f;
  return orig;
}

void Component::setPosition(glm::ivec2 position) {
  this->position = position;
  queueDraw();
}

glm::ivec2 Component::getPosition() const {
  return position;
}

void Component::setAnchorX(Anchor anchor) {
  this->anchorX = anchor;
  queueDraw();
}

Anchor Component::getAnchorX() const {
  return anchorX;
}

void Component::setAnchorY(Anchor anchor) {
  this->anchorY = anchor;
  queueDraw();
}

Anchor Component::getAnchorY() const {
  return anchorY;
}
