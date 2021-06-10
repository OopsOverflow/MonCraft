#pragma once

#include "Block.hpp"

class Gravel_Block : public Block {
public:
    static Gravel_Block* get() {
      static Gravel_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {1, 3};
    }

private:
    Gravel_Block() : Block(BlockType::Gravel) {}
};
