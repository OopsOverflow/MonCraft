#ifndef GRASS_BLOCK_H
#define GRASS_BLOCK_H

#include "Block.h"

class Grass_Block : public Block
{
public:
    Grass_Block();

    std::vector<GLfloat> getSideTxrCoords();
    std::vector<GLfloat> getTopTxrCoords();
    std::vector<GLfloat> getBottomTxrCoords();

};

#endif // GRASS_BLOCK_H