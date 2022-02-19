#pragma once

#include <cstdint>
#include <string>

namespace prng {
  void srand(uint32_t seed);
  uint32_t srands(std::string const& seed);
  uint32_t hashString(std::string const& s);
  uint32_t rand();
}
