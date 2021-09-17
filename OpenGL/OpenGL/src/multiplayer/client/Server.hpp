#pragma once

#include <string>
#include "SharedEntities.hpp"

class Server {

public:
  Server();
  virtual ~Server();

  virtual void ping();
  virtual void update();
  entities_ptr_t getEntities() const;

protected:
  entities_ptr_t entities;
};
