#pragma once
#include "Game.hpp"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	char lastKeyClicked = 'd';
	int rifleCooldown = 0;
	int rocketLauncherCooldown = 0;
	TransformComponent* transform;
	SpriteComponent* sprite;
	bool weaponReady = true;
	KeyboardController(){}
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		
	}

	void update() override
	{	
		rifleCooldown--;
		rocketLauncherCooldown--;
		Vector2D playerPosition = transform->position;
		int weapon = Game::playerWeapon;
		switch(weapon){
		case Game::rocketLauncher:
			if (Game::event.type == SDL_KEYDOWN)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_w:
					transform->velocity.y = -1;
					sprite->play("RocketLauncherWalkUp");
					lastKeyClicked = 'w';
					break;
				case SDLK_a:
					transform->velocity.x = -1;
					sprite->play("RocketLauncherWalkSideways");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					lastKeyClicked = 'a';
					break;
				case SDLK_d:
					transform->velocity.x = 1;
					sprite->play("RocketLauncherWalkSideways");
					sprite->spriteFlip = SDL_FLIP_NONE;
					lastKeyClicked = 'd';
					break;
				case SDLK_s:
					transform->velocity.y = 1;
					sprite->play("RocketLauncherWalkDown");
					lastKeyClicked = 's';
					break;
				case SDLK_e:
					Game::playerWeapon = Game::rifle;
					break;
				case SDLK_q:
					Game::playerWeapon = Game::rifle;
					break;
				case SDLK_SPACE:
					if ((weaponReady) && (Game::rocketAmmunition>0)&&(rocketLauncherCooldown<=0)) {


						switch (lastKeyClicked)
						{
						case 'w':
							Game::assets->createProjectile(Game::rocketLauncher, playerPosition + Vector2D(52, -30), Vector2D(0, -3), 1000, 8, "RocketLauncherProjectileUp", SDL_FLIP_NONE);
							sprite->play("RocketLauncherShootUp");
							break;
						case 'a':
							Game::assets->createProjectile(Game::rocketLauncher, playerPosition + Vector2D(-30, 52), Vector2D(-3, 0), 1000, 8, "RocketLauncherProjectileSideways", SDL_FLIP_HORIZONTAL);
							sprite->play("RocketLauncherShootSideways");
							break;
						case 's':
							Game::assets->createProjectile(Game::rocketLauncher, playerPosition + Vector2D(32, 62), Vector2D(0, 3), 1000, 8, "RocketLauncherProjectileUp", SDL_FLIP_VERTICAL);
							sprite->play("RocketLauncherShootDown");
							break;
						case 'd':
							Game::assets->createProjectile(Game::rocketLauncher, playerPosition + Vector2D(96, 52), Vector2D(3, 0), 1000, 8, "RocketLauncherProjectileSideways", SDL_FLIP_NONE);
							sprite->play("RocketLauncherShootSideways");
							break;
						default:
							break;
						}
						weaponReady = false;
						Game::rocketAmmunition--;
						rocketLauncherCooldown = 60;
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
						sprite->play("RocketLauncherIdle");
					}
					break;
				case SDLK_a:
					if (transform->velocity.x < 0) {
						transform->velocity.x = 0;
						sprite->play("RocketLauncherIdle");
					}
					break;
				case SDLK_d:
					if (transform->velocity.x > 0) {
						transform->velocity.x = 0;
						sprite->play("RocketLauncherIdle");
					}
					break;
				case SDLK_s:
					if (transform->velocity.y > 0) {
						transform->velocity.y = 0;
						sprite->play("RocketLauncherIdle");
					}
					break;
				case SDLK_SPACE:
					sprite->play("RocketLauncherIdle");
					weaponReady = true;
					break;
				default:
					break;
				}
			}
			break;
		case Game::rifle:
			if (Game::event.type == SDL_KEYDOWN)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_w:
					transform->velocity.y = -1;
					sprite->play("RifleWalkUp");
					lastKeyClicked = 'w';
					break;
				case SDLK_a:
					transform->velocity.x = -1;
					sprite->play("RifleWalkSideways");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					lastKeyClicked = 'a';
					break;
				case SDLK_d:
					transform->velocity.x = 1;
					sprite->play("RifleWalkSideways");
					sprite->spriteFlip = SDL_FLIP_NONE;
					lastKeyClicked = 'd';
					break;
				case SDLK_s:
					transform->velocity.y = 1;
					sprite->play("RifleWalkDown");
					lastKeyClicked = 's';
					break;
				case SDLK_e:
					Game::playerWeapon = Game::rocketLauncher;
					break;
				case SDLK_q:
					Game::playerWeapon = Game::rocketLauncher;
					break;
				case SDLK_SPACE:
					if(rifleCooldown<=0){
						switch (lastKeyClicked)
						{
						case 'w':
							Game::assets->createProjectile(Game::rifle, playerPosition + Vector2D(52, -30), Vector2D(0, -3), 1000, 8, "RifleProjectileUp", SDL_FLIP_NONE);
							sprite->play("RifleShootUp");
							break;
						case 'a':
							Game::assets->createProjectile(Game::rifle, playerPosition + Vector2D(-30, 52), Vector2D(-3, 0), 1000, 8, "RifleProjectileSideways", SDL_FLIP_HORIZONTAL);
							sprite->play("RifleShootSideways");
							break;
						case 's':
							Game::assets->createProjectile(Game::rifle, playerPosition + Vector2D(32, 62), Vector2D(0, 3), 1000, 8, "RifleProjectileUp", SDL_FLIP_VERTICAL);
							sprite->play("RifleShootDown");
							break;
						case 'd':
							Game::assets->createProjectile(Game::rifle, playerPosition + Vector2D(96, 52), Vector2D(3, 0), 1000, 8, "RifleProjectileSideways", SDL_FLIP_NONE);
							sprite->play("RifleShootSideways");
							break;
						default:
							break;
						}
						rifleCooldown = 10;
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
						sprite->play("RifleIdle");
					}
					break;
				case SDLK_a:
					if (transform->velocity.x < 0) {
						transform->velocity.x = 0;
						sprite->play("RifleIdle");
					}
					break;
				case SDLK_d:
					if (transform->velocity.x > 0) {
						transform->velocity.x = 0;
						sprite->play("RifleIdle");
					}
					break;
				case SDLK_s:
					if (transform->velocity.y > 0) {
						transform->velocity.y = 0;
						sprite->play("RifleIdle");
					}
					break;
				case SDLK_SPACE:
					sprite->play("RifleIdle");
					break;
				default:
					break;
				}
			}
			break;
		}
		
	}
};