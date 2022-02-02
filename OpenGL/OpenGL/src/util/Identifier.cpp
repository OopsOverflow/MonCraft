#include "Identifier.hpp"

#include <random>

static std::random_device rd;
static std::mt19937_64 gen(rd());

Identifier generateIdentifier() {
  return Identifier(gen());
}
