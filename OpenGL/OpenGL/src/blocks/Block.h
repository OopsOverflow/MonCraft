#ifndef BLOCK_H
#define BLOCK_H

#include "GL/glew.h"
#include <vector>

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone
};

class Block
{
public:
    Block(BlockType type);

    virtual ~Block() { }

    const BlockType type;

    virtual std::vector<GLfloat> getSideTexCoords() const = 0;
    virtual std::vector<GLfloat> getTopTexCoords() const = 0;
    virtual std::vector<GLfloat> getBottomTexCoords() const = 0;

};

#endif // BLOCK_H
