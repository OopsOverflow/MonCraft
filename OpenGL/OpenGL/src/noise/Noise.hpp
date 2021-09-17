#pragma once

// COMBAK: Do we keep the value noise ? It is probably worse, but much faster
// than the new XXHash. We may want to build a common api to the two noises
// (and others) to switch easily between them, just in the cpp file.
#include "../noise/value.hpp"
#include "../noise/XXHash.hpp"
#include "../noise/voronoi.hpp"
#include "../noise/simplex.hpp"
#include "../noise/bitmap.hpp"
#include "../noise/bitmapTex.hpp"
#include "../noise/pipeline.hpp"
