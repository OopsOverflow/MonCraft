#pragma once

#include <SFML/Config.hpp>

/**
 * UUID for entities and other network-shared resources.
 */
 using Identifier = sf::Uint64;

 Identifier generateIdentifier();
