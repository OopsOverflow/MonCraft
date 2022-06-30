#pragma once

#include <stddef.h>
#include <memory>
#include <unordered_map>

#include "util/Identifier.hpp"

class Entity;

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

  void updateAll(uint32_t dt);
  void renderAll();

  size_t count();
};
