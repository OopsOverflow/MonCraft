#include "Dirt_Block.h"

Dirt_Block::Dirt_Block()
    : Block(Block_Type::Dirt)
{}

//Texture
std::vector<GLfloat> Dirt_Block::getSideTxrCoords()
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
std::vector<GLfloat> Dirt_Block::getTopTxrCoords()
{
    static const std::vector<GLfloat> topTextures =
    {
        // TODO
    };
    return topTextures;
}

std::vector<GLfloat> Dirt_Block::getBottomTxrCoords()
{
    static const std::vector<GLfloat> bottomTextures =
    {
        // TODO
    };

    return bottomTextures;
}
