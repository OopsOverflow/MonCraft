#pragma once

#include "Block.hpp"

class Glass_Block : public Block {
public:
    static Glass_Block* get() {
      static Glass_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {5, 0};
    }

    bool isTransparent() const override { return true; }

private:
    Glass_Block() : Block(BlockType::Glass) {}
};
