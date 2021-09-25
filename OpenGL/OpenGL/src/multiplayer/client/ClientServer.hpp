#pragma once
#include "Server.hpp"

class ClientServer : public Server {

public:
  ClientServer();
  virtual ~ClientServer();

  void ping() override;
  void update() override;
  std::shared_ptr<Character> getPlayer() override;

private:
  std::shared_ptr<Character> player;
};
