#pragma once

#ifdef MONCRAFT_CLIENT
#include "character/CharacterMesh.hpp"
using CharacterImpl = CharacterMesh;
#else
#include "character/Character.hpp"
using CharacterImpl = Character;
#endif

