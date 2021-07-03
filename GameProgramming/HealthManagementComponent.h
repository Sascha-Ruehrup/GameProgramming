#pragma once

#include "ECS.h"
#include "SDL.h"
#include "Components.h"

class HealthManagementComponent : public Component {
private:
	

public:
	int maximumHealth = 1;
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;
	KeyboardController* keyBoardController;
	~HealthManagementComponent(){}
	HealthManagementComponent(int maxH) : maximumHealth(maxH)
	{}
	
	
};
