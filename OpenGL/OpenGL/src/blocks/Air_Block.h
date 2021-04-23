#pragma once

#include "Block.h"

class Air_Block : public Block
{
public:
    static Air_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Air_Block();
};
