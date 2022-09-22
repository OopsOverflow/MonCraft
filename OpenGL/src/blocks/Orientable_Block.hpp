#pragma once

#include "Block.hpp"
#ifdef MONCRAFT_CLIENT
    #include "model/OrientableModel.hpp"
#endif

class Orientable_Block : public Block {
public:
    #ifdef MONCRAFT_CLIENT
        OrientableModel* getModel() const override { return OrientableModel::get(); }
    #endif

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
