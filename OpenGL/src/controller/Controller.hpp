#pragma once

#include <memory>
#include "entity/Entity.hpp"

class Controller {
public:
  Controller();

  virtual void update() = 0;

protected:
  std::unique_ptr<Entity> entity;
};
