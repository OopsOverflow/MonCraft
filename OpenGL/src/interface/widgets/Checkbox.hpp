#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "ui/Button.hpp"
#include "ui/Image.hpp"

class Checkbox : public ui::Button {

public:
  static std::unique_ptr<Checkbox> create();

  void setChecked(bool checked);
  bool getChecked() const;

protected:
  Checkbox();

private:
  std::shared_ptr<ui::Image> img;
  bool checked;
  glm::vec2 pos;
};
