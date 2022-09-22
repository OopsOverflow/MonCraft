#pragma once

#include <memory>
#include "entity/Entity.hpp"

class Controller {
public:
  Controller(std::shared_ptr<Entity> entity);

  std::shared_ptr<Entity> getEntity() {return entity;}

  virtual void update() = 0;

protected:
  std::shared_ptr<Entity> entity;
};
