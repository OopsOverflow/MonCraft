#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "geometry/DefaultGeometry.hpp"

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone,
    Leaf,
    Wood,
    Ice,
    Sand,
    Sandstone,
    Snow,
    Tallgrass,
    Water
};

enum class BlockFace { TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT };

class Block
{
public:
    Block(BlockType type);

    virtual ~Block() { }

    const BlockType type;

    virtual glm::ivec2 getFaceUVs(BlockFace face) const = 0;
    virtual bool isStatic() const { return true; }
    virtual bool isVisible() const { return true; }
    virtual bool isSolid() const { return true; }
    virtual bool isTransparent() const { return false; }
    virtual BlockGeometry* getGeometry() const { return DefaultBlockGeometry::get(); }

private:
    /**
    * Properly deletes smart pointers of static or dynamic blocks.
    * /!\ This might be a bad design, may come back to it.
    */
    class Deleter {
    public:
      void operator()(Block* block) const {
        if(!block->isStatic())
          delete block;
      }
    };

public:
    using unique_ptr_t = std::unique_ptr<Block, Deleter>;

    /**
    * convenience to create static blocks
    */
    template<class T, class... Args>
    static
    typename std::enable_if<std::is_base_of<Block, T>::value, unique_ptr_t>::type
    create_static() {
      return unique_ptr_t(T::get());
    }

    /**
    * convenience to create dynamic blocks
    */
    template<class T, class... Args>
    static
    typename std::enable_if<std::is_base_of<Block, T>::value, unique_ptr_t>::type
    create_dynamic(Args&&... args) {
      return unique_ptr_t(new T(std::forward<Args>(args)...));
    }
};
