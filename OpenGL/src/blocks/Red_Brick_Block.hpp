#pragma once

#include "Block.hpp"

class Red_Brick_Block : public Block {
public:
    static Red_Brick_Block* get() {
      static Red_Brick_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {3, 3};
    }

private:
    Red_Brick_Block() : Block(BlockType::Red_Brick) {}
};
