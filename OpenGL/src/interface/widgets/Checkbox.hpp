#pragma once

#include <memory>

#include "ui/Component.hpp"
#include "ui/Image.hpp"

class Checkbox : public ui::Component {

public:
  static std::unique_ptr<Checkbox> create();

  void setChecked(bool checked);
  bool getChecked() const;

protected:
  Checkbox();

private:
  std::unique_ptr<ui::Image> img;
  bool checked;
};
