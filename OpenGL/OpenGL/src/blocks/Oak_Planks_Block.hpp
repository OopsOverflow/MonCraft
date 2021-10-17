#pragma once

#include "Block.hpp"

class Oak_Planks_Block : public Block {
public:
    static Oak_Planks_Block* get() {
      static Oak_Planks_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {7, 1};
    }

private:
    Oak_Planks_Block() : Block(BlockType::Oak_Planks) {}
};
