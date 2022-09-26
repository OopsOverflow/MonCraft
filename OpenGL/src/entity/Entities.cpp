#include "Entities.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <utility>

#include "entity/Entity.hpp"
#include "save/SaveManager.hpp"
#include "debug/Debug.hpp"

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
  
  static const float collisionStrength = 1.f * dt;
  for(auto it1 = begin(); it1 != end(); ++it1) {
    for(auto it2 = it1; it2 != end(); ++it2) {
      if (it1 == it2) continue;
      std::shared_ptr<Entity> e1 = it1->second;
      std::shared_ptr<Entity> e2 = it2->second;
      auto p1 = e1->getPosition();
      auto p2 = e2->getPosition();
      if(e1->hitbox.collides(p1, p2, e2->hitbox)) {
        p1.y *= 0.1f;
        p2.y *= 0.1f;
        auto axis = normalize(p2 - p1);
        e1->speed -= axis * collisionStrength;
        e2->speed += axis * collisionStrength;
      }
    }
  }
}
void Entities::renderAll() {
  for(auto& entity : *this)
    entity.second->render();
}

size_t Entities::count() {
  return size();
}


void Entities::save() {
    spdlog::debug("Saving {} entities...", count());
  for(auto& pair: *this) {
    // TODO: not all entities are characters!
    SaveManager::saveEntity(*pair.second, EntityType::Character, pair.first);
  }
}