#pragma once

#include "Block.h"

class Ice_Block : public Block
{
public:
    static Ice_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Ice_Block();
};
