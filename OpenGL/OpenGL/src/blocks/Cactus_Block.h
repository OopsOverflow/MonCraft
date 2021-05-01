#pragma once

#include "Block.h"

class Cactus_Block : public Block
{
public:
    static Cactus_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Cactus_Block();
};
