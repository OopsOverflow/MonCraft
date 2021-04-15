#ifndef DIRT_BLOCK_H
#define DIRT_BLOCK_H

#include "Block.h"


class Dirt_Block : public Block
{
public:
    Dirt_Block();

    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
};

#endif // DIRT_BLOCK_H
