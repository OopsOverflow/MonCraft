#pragma once

#include <memory>
#include "entity/character/Character.hpp"
#include "util/Identifier.hpp"

class Server {

public:
  Server() = default;
  virtual ~Server() = default;

  virtual void ping() = 0;
  virtual void update() = 0;

  /**
   * Returns nullptr if the player was not created.
   */
  virtual std::shared_ptr<Character> getPlayer() = 0;
};
