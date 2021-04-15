#ifndef BLOCK_H
#define BLOCK_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <vector>

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone
};

enum class BlockFace { TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT };

class Block
{
public:
    Block(BlockType type);

    virtual ~Block() { }

    const BlockType type;

    virtual glm::ivec2 getFaceUVs(BlockFace face) const = 0;

};

#endif // BLOCK_H
