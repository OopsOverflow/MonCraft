#pragma once

#include "Block.hpp"
#include "Debug_Block.hpp"

#include "Air_Block.hpp"
#include "Grass_Block.hpp"
#include "Dirt_Block.hpp"
#include "Stone_Block.hpp"
#include "Leaf_Block.hpp"
#include "Wood_Block.hpp"
#include "Ice_Block.hpp"
#include "Sand_Block.hpp"
#include "Sandstone_Block.hpp"
#include "Snow_Block.hpp"
#include "Tallgrass_Block.hpp"
#include "Water_Block.hpp"
#include "Cactus_Block.hpp"

#include <sstream>

class AllBlocks {

public:
  AllBlocks() = delete;

  /**
  * convenience to create static blocks.
  * /!\ Take care ! using this function demands that the block has been
  * correctly inserted it the array.
  */
  static Block::unique_ptr_t create_static(BlockType type) {
    static std::array<Block*(*)(), 13> factories {
      (Block*(*)())Air_Block::get,
      (Block*(*)())Grass_Block::get,
      (Block*(*)())Dirt_Block::get,
      (Block*(*)())Stone_Block::get,
      (Block*(*)())Leaf_Block::get,
      (Block*(*)())Wood_Block::get,
      (Block*(*)())Ice_Block::get,
      (Block*(*)())Sand_Block::get,
      (Block*(*)())Sandstone_Block::get,
      (Block*(*)())Snow_Block::get,
      (Block*(*)())Tallgrass_Block::get,
      (Block*(*)())Water_Block::get,
      (Block*(*)())Cactus_Block::get,
    };
    auto index = (size_t)type;
    if(index > factories.size()) {
      std::ostringstream err;
      err << "create_static: BlockType not recognized: " << index;
      throw std::runtime_error(err.str());
    }
    return Block::unique_ptr_t(factories[index]());
  }
};
