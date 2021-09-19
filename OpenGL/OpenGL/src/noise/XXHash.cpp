#include "XXHash.hpp"

using namespace glm; // TODO remove immediately

XXHash::XXHash(uint32_t seed)
  : seed(seed)
{ }

template<>
uint32_t XXHash::sample<1, 1>(int32_t val) const {
  uint32_t h32 = seed + PRIME32_5;
  h32 += 4;
  h32 += val * PRIME32_3;
  h32 = rotateLeft (h32, 17) * PRIME32_4;
  h32 ^= h32 >> 15;
  h32 *= PRIME32_2;
  h32 ^= h32 >> 13;
  h32 *= PRIME32_3;
  h32 ^= h32 >> 16;
  return h32;
}

template<glm::length_t IN>
uint32_t XXHash::sample1D(typename in_t<IN>::type val) const {
  uint32_t h32 = seed + PRIME32_5 + IN * 4;

  for(auto i = 0; i < IN; i++) {
    h32 += val[i] * PRIME32_3;
    h32 = rotateLeft(h32, 17) * PRIME32_4;
  }

  h32 ^= h32 >> 15;
  h32 *= PRIME32_2;
  h32 ^= h32 >> 13;
  h32 *= PRIME32_3;
  h32 ^= h32 >> 16;

  return h32;
}

template<glm::length_t IN>
XXHash::out_t<2>::type XXHash::sample2D(typename XXHash::in_t<IN>::type val) const {
  uint32_t h32 = sample<IN, 1>(val);

  glm::vec<2, uint16_t> res{
      h32 & UINT16_MAX,
      (h32 >> 16) & UINT16_MAX,
  };

  return res;
}

template<glm::length_t IN>
XXHash::out_t<3>::type XXHash::sample3D(typename XXHash::in_t<IN>::type val) const {
  uint32_t h32 = sample<IN, 1>(val);

  glm::vec<3, uint8_t> res{
      h32 & UINT8_MAX,
      (h32 >> 8) & UINT8_MAX,
      (h32 >> 16) & UINT8_MAX,
  };

  return res;
}

#define GEN_TEMPLATE(IN, OUT, FN)                                              \
  template <>                                                                  \
  typename XXHash::out_t<OUT>::type XXHash::sample<IN, OUT>(                   \
      typename XXHash::in_t<IN>::type val) const {                             \
    return FN(val);                                                            \
  }

GEN_TEMPLATE(2, 1, sample1D<2>)
GEN_TEMPLATE(3, 1, sample1D<3>)
GEN_TEMPLATE(1, 2, sample2D<1>)
GEN_TEMPLATE(2, 2, sample2D<2>)
GEN_TEMPLATE(3, 2, sample2D<2>)
GEN_TEMPLATE(1, 3, sample3D<1>)
GEN_TEMPLATE(2, 3, sample3D<2>)
GEN_TEMPLATE(3, 3, sample3D<3>)

#undef GEN_TEMPLATE1

uint32_t XXHash::rotateLeft(uint32_t val, int32_t count) const {
  return (val << count) | (val >> (32 - count));
}
