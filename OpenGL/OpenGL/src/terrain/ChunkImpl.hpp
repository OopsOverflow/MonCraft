#pragma once

#ifdef MONCRAFT_CLIENT
#include "ChunkMesh.hpp"
using ChunkImpl = ChunkMesh;
#else
#include "Chunk.hpp"
using ChunkImpl = Chunk;
#endif
