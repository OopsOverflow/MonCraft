#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "ui/Button.hpp"
#include "ui/Image.hpp"

#include "blocks/AllBlocks.hpp"

class BlockSelector : public ui::Button {

public:
  static std::unique_ptr<BlockSelector> create();

  void setSelected(bool selected);
  bool getSelected() const;

  void setBlock(BlockType block);
  BlockType getBlock() const;

protected:
  BlockSelector();

private:
  std::shared_ptr<ui::Image> img;
  BlockType block;
  bool selected;
  glm::vec2 pos;
};
