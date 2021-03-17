#include "Stone_Block.h"

Stone_Block::Stone_Block()
    : Block(Block_Type::Stone)
{}


std::vector<GLfloat> Stone_Block::getSideTxrCoords()
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
std::vector<GLfloat> Stone_Block::getTopTxrCoords()
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}

std::vector<GLfloat> Stone_Block::getBottomTxrCoords()
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}