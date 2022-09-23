#include "Entities.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <utility>

#include "entity/Entity.hpp"

Entities::Entities()
{}


Entities::~Entities()
{}

std::shared_ptr<Entity> Entities::get(Identifier uid) {
  auto found = find(uid);
  if(found == end()) {
    spdlog::warn("Entity not found: {}", uid);
    return nullptr;
  }
  else return found->second;
}

std::shared_ptr<Entity> Entities::add(Identifier uid, std::unique_ptr<Entity> entity) {
  spdlog::debug("Added entity to Entities list: {}", uid);
  auto res = emplace(uid, std::move(entity));
  return res.first->second;
}

bool Entities::remove(Identifier uid) {
  bool res = erase(uid);
  return res;
}


void Entities::updateAll(uint32_t dt) {
  for(auto& entity : *this)
    entity.second->update(dt);
}
void Entities::renderAll() {
  for(auto& entity : *this)
    entity.second->render();
}

size_t Entities::count() {
  return size();
}
