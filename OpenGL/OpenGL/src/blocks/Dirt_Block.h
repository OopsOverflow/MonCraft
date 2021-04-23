#pragma once

#include "Block.h"

class Dirt_Block : public Block
{
public:
    static Dirt_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Dirt_Block();
};
