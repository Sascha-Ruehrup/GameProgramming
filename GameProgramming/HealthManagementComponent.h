#pragma once

#include "ECS.h"
#include "SDL.h"
#include "Components.h"

class HealthManagementComponent : public Component {
public:
	int maximumHealth = 1;

	~HealthManagementComponent(){}
	HealthManagementComponent(int maxH) : maximumHealth(maxH)
	{}
	
	
};
