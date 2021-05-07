#pragma once

#include "Block.hpp"

class Air_Block : public Block {
public:
    static Air_Block* get() {
      static Air_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {0, 0};
    }

private:
    Air_Block() : Block(BlockType::Air) {}
};