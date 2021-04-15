#ifndef STONE_BLOCK_H
#define STONE_BLOCK_H

#include "Block.h"

class Stone_Block : public Block
{
public:
    Stone_Block();

    glm::ivec2 getFaceUVs(BlockFace face) const override;
};

#endif // STONE_BLOCK_H
