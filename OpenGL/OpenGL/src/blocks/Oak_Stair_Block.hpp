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

    virtual std::ostream& serialize(std::ostream &stream) const override {
        stream << facing;
        return stream;
    }

    virtual Block* deserialize(std::istream &stream) override {
        Facing facing;
        stream >> facing;
        return new Oak_Stair_Block(facing);
    }

private:
    Oak_Stair_Block() : Orientable_Block(BlockType::Oak_Stair, Facing::NORTH, true) {}
public:
    Oak_Stair_Block(Facing facing) : Orientable_Block(BlockType::Oak_Stair, facing, false) {}
};
