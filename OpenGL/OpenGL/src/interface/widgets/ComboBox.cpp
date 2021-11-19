#include "ComboBox.hpp"

#include "ui/Box.hpp"
#include "ui/Image.hpp"

using namespace ui;

ComboBox::ComboBox(std::vector<std::string> options)
  : options(options)
{
  auto box = Box::create();
  box->pack_start(Image::create({0, 0}, {10, 10}));

}
