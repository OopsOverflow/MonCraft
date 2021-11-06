#pragma once

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

  static std::ostream& serialize(std::ostream& stream, Block::unique_ptr_t const& block);
  static Block::unique_ptr_t deserialize(std::istream& stream);

  static BlockType nextBlock(BlockType type);

private:
  static const size_t BlockCount = 23;

  static const std::array<Block*(*)(), BlockCount> factories;
};
