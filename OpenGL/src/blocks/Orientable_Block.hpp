#pragma once

#include "Block.hpp"
#include "model/OrientableModel.hpp"

class Orientable_Block : public Block {
public:
    OrientableModel* getModel() const override { return OrientableModel::get(); }

    Facing getFacing() const { return facing; }

protected:
    Facing facing;

    Orientable_Block(BlockType type)
        : Block(type, true), facing(Facing::NORTH)
    {}

    Orientable_Block(BlockType type, Facing facing)
        : Block(type, false), facing(facing)
    {}
};
