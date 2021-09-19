#pragma once

#include <cstdint>
#include <glm/glm.hpp>

// credit: based on implementation of xxhash proposed by Rune Skovbo Johansen
// http://blog.runevision.com/2015/01/primer-on-repeatable-random-numbers.html
// https://hg.sr.ht/~runevision/random-numbers-testing/browse/Assets/Implementations/HashFunctions/XXHash.cs?rev=tip


/**
 * A non-iterative ND-space value noise generator.
 * Generates reproducible noise based on N-dimensional coordinates and a seed.
 */

class XXHash {
private:
	template<glm::length_t L> struct in_t {};
	template<glm::length_t L> struct out_t {};

	uint32_t seed;

	static const uint32_t PRIME32_1 = 2654435761;
	static const uint32_t PRIME32_2 = 2246822519;
	static const uint32_t PRIME32_3 = 3266489917;
	static const uint32_t PRIME32_4 = 668265263;
	static const uint32_t PRIME32_5 = 374761393;

public:
	XXHash(uint32_t seed);

	uint32_t sample(int32_t val) const;

	/**
	* Samples a point in IN-dimensional space and returns a OUT-uplet of pseudo-random
	* linearly distributed integers in the range [0, UINT_OUT_MAX]
	*/
	template<glm::length_t IN, glm::length_t OUT>
 	typename out_t<OUT>::type sample(typename in_t<IN>::type val) const;


private:
	// 1 template parameter
	template<glm::length_t IN>
	uint32_t sample1D(typename in_t<IN>::type val) const;
	template<glm::length_t IN>
	glm::vec<2, uint16_t> sample2D(typename in_t<IN>::type val) const;
	template<glm::length_t IN>
	glm::vec<3, uint8_t> sample3D(typename in_t<IN>::type val) const;

	uint32_t rotateLeft(uint32_t val, int32_t count) const;
};

// inputs are int or vectors of int
template<> struct XXHash::in_t<3> { using type = glm::vec<3, int>; };
template<> struct XXHash::in_t<2> { using type = glm::vec<2, int>; };
template<> struct XXHash::in_t<1> { using type = int; };

// outputs are uint or vec of uint (reduced precision)
template<> struct XXHash::out_t<3> { using type = glm::vec<3, uint8_t>; };
template<> struct XXHash::out_t<2> { using type = glm::vec<2, uint16_t>; };
template<> struct XXHash::out_t<1> { using type = uint32_t; };

// 1 template parameter
// template<> uint32_t XXHash::sample1D<2>(XXHash::in_t<2>::type val) const;
// template<> uint32_t XXHash::sample1D<3>(XXHash::in_t<3>::type val) const;
// template<> glm::vec<2, uint16_t> XXHash::sample2D<1>(XXHash::in_t<1>::type val) const;
// template<> glm::vec<2, uint16_t> XXHash::sample2D<2>(XXHash::in_t<2>::type val) const;
// template<> glm::vec<2, uint16_t> XXHash::sample2D<3>(XXHash::in_t<3>::type val) const;
// template<> glm::vec<3, uint8_t> XXHash::sample3D<1>(XXHash::in_t<1>::type val) const;
// template<> glm::vec<3, uint8_t> XXHash::sample3D<2>(XXHash::in_t<2>::type val) const;
// template<> glm::vec<3, uint8_t> XXHash::sample3D<3>(XXHash::in_t<3>::type val) const;


// 2 templates parameters
template<> XXHash::out_t<1>::type XXHash::sample<1, 1>(XXHash::in_t<1>::type val) const;
template<> XXHash::out_t<1>::type XXHash::sample<2, 1>(XXHash::in_t<2>::type val) const;
template<> XXHash::out_t<1>::type XXHash::sample<3, 1>(XXHash::in_t<3>::type val) const;
template<> XXHash::out_t<2>::type XXHash::sample<1, 2>(XXHash::in_t<1>::type val) const;
template<> XXHash::out_t<2>::type XXHash::sample<2, 2>(XXHash::in_t<2>::type val) const;
template<> XXHash::out_t<2>::type XXHash::sample<3, 2>(XXHash::in_t<3>::type val) const;
template<> XXHash::out_t<3>::type XXHash::sample<1, 3>(XXHash::in_t<1>::type val) const;
template<> XXHash::out_t<3>::type XXHash::sample<2, 3>(XXHash::in_t<2>::type val) const;
template<> XXHash::out_t<3>::type XXHash::sample<3, 3>(XXHash::in_t<3>::type val) const;
