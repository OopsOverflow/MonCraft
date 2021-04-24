#pragma once

#include "Hitbox.hpp"
#include "Body.hpp"
#include "../audio/SoundEffect.hpp"

class Entity
{
public:
	Entity();
	virtual ~Entity();
	virtual void move();

private:

};