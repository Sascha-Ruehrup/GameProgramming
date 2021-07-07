#pragma once

#include "Components.h"
#include "SDL.h"
#include "Texturemanager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	

	bool animated = false;
	int frames = 0;
	int speed = 100;
	double angle = 0.0;
public:
	int animationIndex = 0;
	SDL_Rect srcRect, destRect;
	std::map<const char*, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}
	SpriteComponent(std::string id,SDL_RendererFlip flip)
	{
		spriteFlip = flip;
		setTex(id);
	}
	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;
		if (animated) {
			Animation rifleIdleSideways = Animation(1, 4, 200);
			Animation rifleIdleUp = Animation(17, 4, 100);
			Animation rifleIdleDown = Animation(16, 3, 100);

			Animation rifleWalkSideways = Animation(2, 10, 50);
			Animation rifleWalkUp = Animation(0, 4, 200);
			Animation rifleWalkDown = Animation(3, 3, 200);

			Animation rifleShootSideways = Animation(4, 4, 50);
			Animation rifleShootUp = Animation(6, 4, 50);
			Animation rifleShootDown = Animation(5, 3, 50);

			Animation rocketLauncherIdleSideways = Animation(8, 4, 50);
			Animation rocketLauncherIdleUp = Animation(14, 4, 100);
			Animation rocketLauncherIdleDown = Animation(15, 3, 100);

			Animation rocketLauncherWalkSidways = Animation(7, 10, 50);
			Animation rocketLauncherWalkUp = Animation(9, 4, 50);
			Animation rocketLauncherWalkDown = Animation(10, 3, 50);

			Animation rocketLauncherShootSideways = Animation(11, 4, 50);
			Animation rocketLauncherShootUp = Animation(12, 4, 50);
			Animation rocketLauncherShootDown = Animation(13, 3, 50);

			animations.emplace("RifleIdleSideways", rifleIdleSideways);
			animations.emplace("RifleIdleUp", rifleIdleUp);
			animations.emplace("RifleIdleDown", rifleIdleDown);

			animations.emplace("RifleWalkSideways", rifleWalkSideways);
			animations.emplace("RifleWalkUp", rifleWalkUp);
			animations.emplace("RifleWalkDown", rifleWalkDown);

			animations.emplace("RifleShootSideways", rifleShootSideways);
			animations.emplace("RifleShootUp", rifleShootUp);
			animations.emplace("RifleShootDown", rifleShootDown);

			animations.emplace("RocketLauncherIdleSideways", rocketLauncherIdleSideways);
			animations.emplace("RocketLauncherIdleUp", rocketLauncherIdleUp);
			animations.emplace("RocketLauncherIdleDown", rocketLauncherIdleDown);

			animations.emplace("RocketLauncherWalkSideways", rocketLauncherWalkSidways);
			animations.emplace("RocketLauncherWalkUp", rocketLauncherWalkUp);
			animations.emplace("RocketLauncherWalkDown", rocketLauncherWalkDown);

			animations.emplace("RocketLauncherShootSideways", rocketLauncherShootSideways);
			animations.emplace("RocketLauncherShootUp", rocketLauncherShootUp);
			animations.emplace("RocketLauncherShootDown", rocketLauncherShootDown);

			play("RifleIdleSideways");
		}
		setTex(id);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string id)
	{
		texture = Game::assets->getTexture(id);
	}
	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		

	}
	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		srcRect.y = animationIndex * transform->height;
		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override
	{
		TextureManager::draw(texture, srcRect, destRect,spriteFlip, angle);
	}
	void play(const char* animationName)
	{
		frames = animations[animationName].frames;
		animationIndex = animations[animationName].index;
		speed = animations[animationName].speed;
	}
	void setAngle(double angle) {
		this->angle = angle;
	}
};