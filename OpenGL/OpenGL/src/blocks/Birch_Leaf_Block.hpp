#pragma once

#include "Block.hpp"

class Birch_Leaf_Block : public Block {
public:
    static Birch_Leaf_Block* get() {
      static Birch_Leaf_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {6, 7};
    }

    bool isTransparent() const override { return true; }

private:
    Birch_Leaf_Block() : Block(BlockType::Birch_Leaf) {}
};
