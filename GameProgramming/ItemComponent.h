#pragma once
#include "ECS.h"
#include "Components.h"

class ItemComponent : public Component
{
public:
	int health = 0;
	int rockets = 0;
	ItemComponent(int health, int rockets){
		this->health = health;
		this->rockets = rockets;
	}
	void addHealthToItem(int health) {
		this->health += health;
	}
	void addRocketsToItem(int rockets) {
		this->rockets += rockets;
	}
};