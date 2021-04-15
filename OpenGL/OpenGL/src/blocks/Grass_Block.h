#ifndef GRASS_BLOCK_H
#define GRASS_BLOCK_H

#include "Block.h"

class Grass_Block : public Block
{
public:
    Grass_Block();

    glm::ivec2 getFaceUVs(BlockFace face) const override;

};

#endif // GRASS_BLOCK_H
