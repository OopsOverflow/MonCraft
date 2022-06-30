#pragma once

#include <stddef.h>
#include <memory>
#include <vector>

#include "ui/Component.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Specification.hpp"
#include "ui/style/Style.hpp"
#include "util/DataStore.hpp"

namespace ui {

class Grid : public Component {

protected:
  Grid();

public:
  static std::unique_ptr<Grid> create();

  static const spec_t GAP;

  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void pack(glm::ivec2 pos, Component* comp);
  void pack(glm::ivec2 pos, std::unique_ptr<Component> comp);

  void draw() override;
  glm::ivec2 size() const { return cells.size(); }

  void setGap(glm::ivec2 gap);
  glm::ivec2 getGap() const;

private:

  class Cell : public Component {
  public:
    static std::unique_ptr<Cell> create(Component* comp);
    static std::unique_ptr<Cell> create(std::unique_ptr<Component> comp);

    bool contains(Component* comp) const;

  private:
    Cell();
  };

  void updateCells();
  void ensureCellExists(glm::ivec2 pos);

  DataStore<std::unique_ptr<Cell>, 2> cells;
};

} // namespace ui
