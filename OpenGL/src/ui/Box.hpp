#pragma once

#include <stddef.h>
#include <memory>
#include <vector>

#include "ui/Component.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Specification.hpp"
#include "ui/style/Style.hpp"

namespace ui {

class Box : public Component {

protected:
  Box();

public:
  static std::unique_ptr<Box> create();

  enum class Orientation { HORIZONTAL, VERTICAL };

  static const spec_t GAP;
  static const spec_t ORIENTATION;

  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void pack(size_t index, std::shared_ptr<Component> comp);
  void pack_start(std::shared_ptr<Component> comp);
  void pack_end(std::shared_ptr<Component> comp);
  void unpack(std::shared_ptr<Component> comp);
  void unpackAt(size_t index);
  void draw() override;

  void setGap(int gap);
  int getGap() const;

  void setOrientation(Orientation orientation);
  Orientation getOrientation() const;

private:

  class Cell : public Component {
  public:
    static std::unique_ptr<Cell> create(std::shared_ptr<Component> comp);

    bool contains(Component* comp) const;

  private:
    Cell();
  };

  void updateCells();

  std::vector<std::shared_ptr<Cell>> cells;
};

} // namespace ui
