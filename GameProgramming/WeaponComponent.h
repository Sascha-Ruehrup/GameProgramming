#pragma once
#include "Game.hpp"
#include <SDL.h>
#include "Components.h"




class WeaponComponent : public Component {
public:
	int weapon = 0;
	WeaponComponent(){}
	WeaponComponent(int wp) : weapon(wp)
	{
	}
	~WeaponComponent(){}
	
};