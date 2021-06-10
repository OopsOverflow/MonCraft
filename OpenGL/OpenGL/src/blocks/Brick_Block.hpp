#pragma once

#include "Block.hpp"

class Brick_Block : public Block {
public:
    static Brick_Block* get() {
      static Brick_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {0, 3};
    }

private:
    Brick_Block() : Block(BlockType::Brick) {}
};
