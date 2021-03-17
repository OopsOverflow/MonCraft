#ifndef DIRT_BLOCK_H
#define DIRT_BLOCK_H

#include "Block.h"


class Dirt_Block : public Block
{
public:
    Dirt_Block();

    std::vector<GLfloat> getSideTextureCoords();
    std::vector<GLfloat> getTopTextureCoords();
    std::vector<GLfloat> getBottomTextureCoords();

private:
};

#endif // DIRT_BLOCK_H