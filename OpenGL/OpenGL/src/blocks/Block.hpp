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
    Oak_Leaf,
    Oak_Wood,
    Ice,
    Sand,
    Sandstone,
    Snow,
    Tallgrass,
    Water,
    Cactus,
    Birch_Wood,
    Birch_Leaf,
    Gravel,
    Brick,
    Cobalt,
    Glass,
    Oak_Planks,
    Birch_Planks,
    Oak_Stair
};

enum class BlockFace { TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT, INNER };

enum class Facing { NORTH, SOUTH, EAST, WEST, REV_NORTH, REV_SOUTH, REV_EAST, REV_WEST };

class Block
{
public:
    Block(BlockType type, bool static_ = true);

    virtual ~Block() { }

    const BlockType type;

    /**
     * gets the location of the face's texture in the texture atlas.
     * ({0, 0} is the top-left texture.)
     */
    virtual glm::ivec2 getFaceUVs(BlockFace face) const = 0;

    /**
     * A block is considered static if it is not instanciable, analogous to
     * tile-entities in Minecraft. A dynamic block is able to store data
     * per block whereas static blocks are all identical but also take no space
     * in memory (only the pointer).
     */
    bool isStatic() const { return static_; }

    /**
     * A visible block will be drawn (invisibles will be discarded by the mesh).
     */
    virtual bool isVisible() const { return true; }

    /**
     * A solid block has collisions and may be hit by raycasts.
     */
    virtual bool isSolid() const { return true; }

    /**
     * A transparent block has an alpha channel. it is sorted to deal properly
     * with draw orders. Internal faces of two adjacent transparent blocks
     * are drawn (see tree leaves).
     */
    virtual bool isTransparent() const { return false; }

    /**
     * An opaque block has no transparency at all, whereas a non-opaque
     * block has some transparent pixels requiring to draw all neigh faces.
     * (e.g. leaves)
     */
    virtual bool isOpaque() const { return true; }

    /**
     * Water block have different physics
     */
    virtual bool isLiquid() const { return false; }

    /**
     * Directional block have a facing property
     */
    virtual bool isDirectional() const { return false; }

    /**
     * Gets the block geometry generator.
     */
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

    bool static_;

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
