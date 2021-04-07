#ifndef STONE_BLOCK_H
#define STONE_BLOCK_H

#include "Block.h"

class Stone_Block : public Block
{
public:
    Stone_Block();

    std::vector<GLfloat> getSideTxrCoords();
    std::vector<GLfloat> getTopTxrCoords();
    std::vector<GLfloat> getBottomTxrCoords();

};

#endif // STONE_BLOCK_H