#include "Dirt_Block.h"

Dirt_Block::Dirt_Block()
    : Block(Block_Type::Dirt)
{}

//Texture
std::vector<GLfloat> Dirt_Block::getSideTexCoords()
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
std::vector<GLfloat> Dirt_Block::getTopTexCoords()
{
    static const std::vector<GLfloat> topTextures =
    {
        // TODO
    };
    return topTextures;
}

std::vector<GLfloat> Dirt_Block::getBottomTexCoords()
{
    static const std::vector<GLfloat> bottomTextures =
    {
        // TODO
    };

    return bottomTextures;
}
