#include "ServerEntity.hpp"

ServerEntity::ServerEntity(Identifier uid, glm::vec3 pos, Hitbox box)
  : Entity(box), uid(uid)
{ }

void ServerEntity::render()
{ }
