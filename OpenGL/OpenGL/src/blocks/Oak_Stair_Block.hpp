#pragma once

#include "Orientable_Block.hpp"

class Oak_Stair_Block : public Orientable_Block {
public:
    static Oak_Stair_Block* get() {
        static Oak_Stair_Block inst;
        return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
        return {7, 1};
    }

private:
    Oak_Stair_Block() : Orientable_Block(BlockType::Oak_Stair, Facing::EAST, true) {}
public:
    Oak_Stair_Block(Facing facing) : Orientable_Block(BlockType::Oak_Stair, facing, false) {}
};
