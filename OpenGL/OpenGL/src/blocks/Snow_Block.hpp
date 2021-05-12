#pragma once

#include "Block.hpp"

class Snow_Block : public Block {
public:
    static Snow_Block* get() {
      static Snow_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {0, 3};
    }

private:
    Snow_Block() : Block(BlockType::Snow) {}
};
