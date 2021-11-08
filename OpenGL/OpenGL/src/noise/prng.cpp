#include "prng.hpp"

#include "noise/XXHash.hpp"

XXHash hash(0);
uint32_t seed;

uint32_t prng::rand() {
  seed = hash.sample(seed);
  return seed;
}

void prng::srand(uint32_t seed) {
  hash = XXHash(seed);
}

uint32_t prng::hashString(std::string const& s) {
  // see: http://www.cse.yorku.ca/~oz/hash.html
  uint32_t hash = 5381;

  for(unsigned char c : s) {
      hash = ((hash << 5) + hash) + c;
  }

  return hash;
}

uint32_t prng::srands(std::string const& s) {
  auto seed = prng::hashString(s);
  srand(seed);
  return seed;
}
