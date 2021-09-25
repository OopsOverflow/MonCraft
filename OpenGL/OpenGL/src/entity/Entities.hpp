#pragma once

#include "util/Identifier.hpp"
#include "Entity.hpp"

#include <unordered_map>

using entityMap_t = std::unordered_map<Identifier, std::shared_ptr<Entity>>;

class Entities : entityMap_t {

public:
  Entities();
  ~Entities();

  using entityMap_t::begin;
  using entityMap_t::end;

  std::shared_ptr<Entity> get(Identifier uid);

  std::shared_ptr<Entity> add(Identifier uid, std::unique_ptr<Entity> entity);

  bool remove(Identifier uid);

  void updateAll(float dt);
  void renderAll();

  size_t count();
};
