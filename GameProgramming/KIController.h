#pragma once
#include "Game.hpp"
#include "ECS.h"
#include "Components.h"

class KIController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}
	void update() override
	{	
		float xDirection = 0.0f;
		float yDirection = 0.0f;
		Vector2D position = transform->position;
		Vector2D* playerPosition = Game::playerPosition;
		float playerxpos = playerPosition->x;
		float playerypos = playerPosition->y;

		if (position.x < playerxpos) {
			xDirection = 0.1f;
		}
		else if (position.x > playerxpos) {
			xDirection = -0.1f;
		}
		else {
			xDirection = 0.0f;
		}
		if (position.y < playerypos) {
			yDirection = 0.1f;
		}
		else if (position.y > playerypos) {
			yDirection = -0.1f;
		}
		else {
			yDirection = 0.0f;
		}

		move(xDirection, yDirection);

	}
	void move(float xVelocity, float yVelocity) {
		transform->velocity.x = xVelocity;
		transform->velocity.y = yVelocity;
	}
};