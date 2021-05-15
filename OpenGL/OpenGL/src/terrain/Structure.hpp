#pragma once

#include "util/DataStore.hpp"
#include "blocks/Block.hpp"
#include "Chunk.hpp"


class Structure {
public:

  using BlockStore = DataStore<BlockType, 3>;

  struct Slice {
    std::shared_ptr<const BlockStore> store;
    glm::ivec3 cpos;
    glm::ivec3 off, start, end;
  };

  using slices_t = std::vector<Slice>;

  virtual ~Structure() = default;
  virtual slices_t spawn(Chunk& chunk, glm::ivec3 pos) const = 0;
  static void applySlice(Chunk& chunk, Slice const& slice);
};


class Tree : public Structure {

public:
  Tree();
  slices_t spawn(Chunk& chunk, glm::ivec3 pos) const override;
  std::shared_ptr<BlockStore> store;
};
