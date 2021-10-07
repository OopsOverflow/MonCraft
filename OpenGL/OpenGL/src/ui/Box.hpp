#pragma once

#include "Component.hpp"

namespace ui {

class Box : public Component {

public:
  Box();
  virtual ~Box();

  static const spec_t GAP;
  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void pack(Component* comp);
  void unpack(Component* comp);

  void draw() override;

  void setGap(int gap);
  int getGap() const;

private:

  class Cell : public Component {
  public:
    Cell(Component* comp);

    bool contains(Component* comp) const;
  };

  void updateCells();

  std::vector<std::unique_ptr<Cell>> cells;
  int gap;
};

} // namespace ui
