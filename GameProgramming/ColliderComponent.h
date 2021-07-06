#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"
#include "Collision.h"
class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	Circle colliderCircle;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}
	ColliderComponent(std::string t, int xpos, int ypos, int size, bool circle)
	{
		if (circle) {
			tag = t;
			colliderCircle.x = xpos;
			colliderCircle.y = ypos;
			colliderCircle.r = size;	//radius
		}
		else {
			tag = t;
			collider.x = xpos;
			collider.y = ypos;
			collider.h = collider.w = size;
		}
	}
	void init() override
	{
		if (!entity->hasComponent <TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (tag == "player" || tag == "projectile" || tag == "zombie") {
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
		else if (tag == "rocketlauncherprojectile") {			// maybe not needed
			colliderCircle.x = static_cast<int>(transform->position.x);
			colliderCircle.y = static_cast<int>(transform->position.y);
		}
	}

};