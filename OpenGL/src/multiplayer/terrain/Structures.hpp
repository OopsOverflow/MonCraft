#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "util/DataStore.hpp"
#include "blocks/Block.hpp"
#include "terrain/Structure.hpp"


class Oak_Tree : public Structure {

public:
    Oak_Tree();
  slices_t spawn(Chunk& chunk, glm::ivec3 pos) const override;
  std::shared_ptr<BlockStore> store;
};

class Birch_Tree : public Structure {

public:
    Birch_Tree();
    slices_t spawn(Chunk& chunk, glm::ivec3 pos) const override;
    std::shared_ptr<BlockStore> store;
};

class Cactus : public Structure {

public:
    Cactus();
    slices_t spawn(Chunk& chunk, glm::ivec3 pos) const override;
    std::shared_ptr<BlockStore> store;
};

class EditPlateforme : public Structure {

public:
    EditPlateforme();
    slices_t spawn(Chunk& chunk, glm::ivec3 pos) const override;
    std::shared_ptr<BlockStore> store;
};
