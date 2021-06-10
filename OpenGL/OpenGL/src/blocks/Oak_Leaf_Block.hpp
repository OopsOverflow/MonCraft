#pragma once

#include "Block.hpp"

class Oak_Leaf_Block : public Block {
public:
    static Oak_Leaf_Block* get() {
      static Oak_Leaf_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {7, 7};
    }

    bool isTransparent() const override { return true; }

private:
    Oak_Leaf_Block() : Block(BlockType::Oak_Leaf) {}
};
