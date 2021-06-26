#pragma once
#include "Game.hpp"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	char lastKeyClicked = 'd';
	TransformComponent* transform;
	SpriteComponent* sprite;
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		Vector2D playerPosition = transform->position;
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->play("WalkUp");
				lastKeyClicked = 'w';
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->play("WalkSideways");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				lastKeyClicked = 'a';
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->play("WalkSideways");
				sprite->spriteFlip = SDL_FLIP_NONE;
				lastKeyClicked = 'd';
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->play("WalkDown");
				lastKeyClicked = 's';
				break;
			case SDLK_SPACE:
				switch (lastKeyClicked)
				{
				case 'w':
					Game::assets->createProjectile(playerPosition + Vector2D(52, -30), Vector2D(0, -3), 1000, 8, "projectile");
					sprite->play("ShootUp");
					break;
				case 'a':
					Game::assets->createProjectile(playerPosition + Vector2D(-30, 52), Vector2D(-3, 0), 1000, 8, "projectile");
					sprite->play("ShootSideways");
					break;
				case 's':
					Game::assets->createProjectile(playerPosition + Vector2D(32, 62), Vector2D(0, 3), 1000, 8, "projectile");
					sprite->play("ShootDown");
					break;
				case 'd':
					Game::assets->createProjectile(playerPosition + Vector2D(96, 52), Vector2D(3, 0), 1000, 8, "projectile");
					sprite->play("ShootSideways");
					break;
				default:
					break;
				}
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				if (transform->velocity.y < 0) {
					transform->velocity.y = 0;
					sprite->play("Idle");
				}
				break;
			case SDLK_a:
				if (transform->velocity.x < 0) {
					transform->velocity.x = 0;
					sprite->play("Idle");
				}
				break;
			case SDLK_d:
				if (transform->velocity.x > 0) {
					transform->velocity.x = 0;
					sprite->play("Idle");
				}
				break;
			case SDLK_s:
				if (transform->velocity.y > 0) {
					transform->velocity.y = 0;
					sprite->play("Idle");
				}
				break;
			case SDLK_SPACE:
				//sprite->play("Idle");
				break;
			default:
				break;
			}
		}
	}
};