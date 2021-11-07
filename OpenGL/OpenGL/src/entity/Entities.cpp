#include "Entities.hpp"
#include <iostream>

Entities::Entities()
{}


Entities::~Entities()
{}

std::shared_ptr<Entity> Entities::get(Identifier uid) {
  auto found = find(uid);
  if(found == end()) {
    std::cout << "[WARN] entity not found: " << uid << std::endl;
    return nullptr;
  }
  else return found->second;
}

std::shared_ptr<Entity> Entities::add(Identifier uid, std::unique_ptr<Entity> entity) {
  auto res = emplace(uid, std::move(entity));
  return res.first->second;
}

bool Entities::remove(Identifier uid) {
  bool res = erase(uid);
  return res;
}


void Entities::updateAll(float dt) {
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
