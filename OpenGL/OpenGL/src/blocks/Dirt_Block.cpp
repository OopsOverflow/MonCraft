#include "Dirt_Block.h"

Dirt_Block::Dirt_Block()
    : Block(Block_Type::Dirt)
{}

//Texture
std::vector<GLfloat> Dirt_Block::getSideTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
std::vector<GLfloat> Dirt_Block::getTopTexCoords() const
{
    static const std::vector<GLfloat> topTextures =
    {
        // TODO
    };
    return topTextures;
}

std::vector<GLfloat> Dirt_Block::getBottomTexCoords() const
{
    static const std::vector<GLfloat> bottomTextures =
    {
        // TODO
    };

    return bottomTextures;
}
