#pragma once

#include <memory>
#include <string>

#include "ui/Component.hpp"

class ComboBox : public ui::Component {

public:
  static std::unique_ptr<ComboBox> create(std::vector<std::string> options);

  void select(size_t index);
  void select(std::string const& option);

  virtual void draw() override;

protected:
  ComboBox(std::vector<std::string> options);

private:
  std::vector<std::string> options;
};
