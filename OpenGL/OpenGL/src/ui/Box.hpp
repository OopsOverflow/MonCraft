#pragma once

#include "Component.hpp"

namespace ui {

class Box : public Component {

public:
  Box();
  virtual ~Box();

  enum class Orientation { HORIZONTAL, VERTICAL };

  static const spec_t GAP;
  static const spec_t ORIENTATION;

  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void pack(size_t index, Component* comp);
  void pack_start(Component* comp);
  void pack_end(Component* comp);
  void unpack(Component* comp);

  void draw() override;

  void setGap(int gap);
  int getGap() const;

  void setOrientation(Orientation orientation);
  Orientation getOrientation() const;

private:

  class Cell : public Component {
  public:
    Cell(Component* comp);

    bool contains(Component* comp) const;
  };

  void updateCells();

  std::vector<std::unique_ptr<Cell>> cells;
  int gap;
  Orientation orientation;
};

} // namespace ui
