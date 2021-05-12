#pragma once

#include "Block.hpp"

class Leaf_Block : public Block {
public:
    static Leaf_Block* get() {
      static Leaf_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {7, 7};
    }

    bool isTransparent() const override { return true; }

private:
    Leaf_Block() : Block(BlockType::Leaf) {}
};
