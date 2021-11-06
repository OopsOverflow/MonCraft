#pragma once

#include "Orientable_Block.hpp"

class Oak_Planks_Block : public Orientable_Block {
public:
    static Oak_Planks_Block* get() {
      static Oak_Planks_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {7, 1};
    }

private:
    Oak_Planks_Block() : Orientable_Block(BlockType::Oak_Planks) {}
public:
    Oak_Planks_Block(Facing facing) : Orientable_Block(BlockType::Oak_Planks, facing) {}
};
