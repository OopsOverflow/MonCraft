#pragma once

#include "Orientable_Block.hpp"

class Birch_Planks_Block : public Orientable_Block {
public:
    static Birch_Planks_Block* get() {
      static Birch_Planks_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {6, 1};
    }

private:
    Birch_Planks_Block() : Orientable_Block(BlockType::Birch_Planks) {}
public:
    Birch_Planks_Block(Facing facing) : Orientable_Block(BlockType::Birch_Planks, facing) {}
};
