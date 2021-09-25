#pragma once

#include <SFML/Network.hpp>
#include <random>

/**
 * UUID for entities and other network-shared resources.
 */
 using Identifier = sf::Uint64;

 Identifier generateIdentifier();
