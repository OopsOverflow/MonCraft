#pragma once

#include <stddef.h>
#include <array>
#include <ostream>

#include "Block.hpp"

class AllBlocks {

public:
  AllBlocks() = delete;

  /**
  * convenience to create static blocks.
  * /!\ Take care ! using this function demands that the block has been
  * correctly inserted it the array.
  */
  static Block::unique_ptr_t create_static(BlockType type);

  static std::ostream& serialize(std::ostream& stream, Block* block);
  static Block::unique_ptr_t deserialize(std::istream& stream);

  static BlockType nextBlock(BlockType type);

  static const size_t BlockCount = 25;

private:
  static const std::array<Block*(*)(), BlockCount> factories;
};
