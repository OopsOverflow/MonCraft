#pragma once

#include "Block.hpp"

class Birch_Planks_Block : public Block {
public:
    static Birch_Planks_Block* get() {
      static Birch_Planks_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {6, 1};
    }

private:
    Birch_Planks_Block() : Block(BlockType::Birch_Planks) {}
};
