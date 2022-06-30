#include "KeySelector.hpp"

#include <SDL2/SDL.h>
#include <memory>
#include <utility>

#include "ui/Text.hpp"
#include "ui/Button.hpp"
#include "ui/Pane.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

KeySelector::KeySelector(Key key)
  : key(key)
{
  auto button = Button::create();
  auto pane = Pane::create();

  pane->setPadding({ 10, 5 });

  auto font = ResourceManager::getFont("roboto");
  text = Text::create(SDL_GetKeyName(key.asKeycode()), font);

  button->onclick([&] { makeActive(); });

  pane->add(text.get());
  button->add(move(pane));
  add(move(button));
}

void KeySelector::setKey(Key key) {
  this->key = key;
  text->setText(SDL_GetKeyName(key.asKeycode()));
}

Key KeySelector::getKey() const {
  return key;
}

void KeySelector::onKeyPressed(Key key) {
  setKey(key);
  unfocus();
}

void KeySelector::onMouseIn(glm::ivec2 pos) {
  if(!isActive())
    text->setColor({1.0, 0.0, 0.0, 1.0});
}

void KeySelector::onMouseOut(glm::ivec2 pos) {
  if(!isActive())
    text->setColor({0.0, 0.0, 0.0, 1.0});
}

bool KeySelector::onActivate() {
  text->setColor({0.0, 1.0, 0.0, 1.0});
  return true;
}
void KeySelector::onDeactivated() {
  if(isHover())
    text->setColor({0.1, 0.0, 0.0, 1.0});
  else
    text->setColor({0.0, 0.0, 0.0, 1.0});
}

std::unique_ptr<KeySelector> KeySelector::create(Key key) {
  auto sel = std::unique_ptr<KeySelector>(new KeySelector(key));
  sel->initialize();
  return sel;
}
