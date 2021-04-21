#pragma once

#include "Block.h"

class Stone_Block : public Block
{
public:
    static Stone_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Stone_Block();
};
