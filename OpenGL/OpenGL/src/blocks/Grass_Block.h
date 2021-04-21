#pragma once

#include "Block.h"

class Grass_Block : public Block
{
public:
    static Grass_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Grass_Block();
};
