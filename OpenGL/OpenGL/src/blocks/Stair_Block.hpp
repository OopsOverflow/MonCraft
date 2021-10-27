#pragma once

#include "Block.hpp"
#include "geometry/StairGeometry.hpp"

class Stair_Block : public Block {
public:
    bool isOpaque() const override { return false; }

    StairGeometry* getGeometry() const override { return StairGeometry::get(); }

    Facing getFacing() const { return facing; }
private:
    Facing facing;

protected:
    Stair_Block(BlockType type, Facing facing, bool static_)
        : Block(type, static_), facing(facing)
    {}
};
