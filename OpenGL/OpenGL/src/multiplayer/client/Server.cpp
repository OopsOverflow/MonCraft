#include "Server.hpp"

Server::Server()
  : entities(SharedEntities::create())
{}

Server::~Server() {}

entities_ptr_t Server::getEntities() const {
  return entities;
}

void Server::ping() {
  std::cout << "clientside server ping!" << std::endl;
}

void Server::update() {

}
