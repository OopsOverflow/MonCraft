#include "ComboBox.hpp"

#include <algorithm>
#include <utility>
#include <memory>
#include <stddef.h>

#include "ui/Box.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/Text.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

ComboBox::ComboBox(std::vector<std::string> _opt, size_t _sel)
  : options(_opt), selected(_sel)
{
  auto box = Box::create();
  box->setOrientation(Box::Orientation::HORIZONTAL);
  box->setGap(10);

  auto left = Image::create({73, 1179}, {20, 20});
  left->setSize({20, 20});
  auto lbtn = Button::create();
  lbtn->onclick([&] () {
    select(selected == 0 ? options.size() - 1 : selected - 1);
  });
  lbtn->add(move(left));
  box->pack_end(move(lbtn));

  text = Text::create(options.at(selected), ResourceManager::getFont("roboto"));
  box->pack_end(text.get());

  auto right = Image::create({51, 1179}, {20, 20});
  right->setSize({20, 20});
  auto rbtn = Button::create();
  rbtn->onclick([this] () {
    select(selected == options.size() - 1 ? 0 : selected + 1);
  });
  rbtn->add(move(right));
  box->pack_end(move(rbtn));

  add(move(box));
}

void ComboBox::select(size_t index) {
  text->setText(options.at(index));
  selected = index;
}

void ComboBox::select(std::string const& option) {
   auto it = std::find(options.begin(), options.end(), option);
   select(std::distance(options.begin(), it));
}

std::unique_ptr<ComboBox> ComboBox::create(std::vector<std::string> options, size_t selected) {
  ComboBox* box = new ComboBox(options, selected);
  box->initialize();
  return std::unique_ptr<ComboBox>(box);
}
