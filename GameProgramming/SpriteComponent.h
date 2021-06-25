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
	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;
		if (animated) {
			Animation idle = Animation(1, 4, 200);	// row, spritecount, ms between sprites
			Animation walkSideways = Animation(2, 10, 50);
			Animation walkUp = Animation(0, 4, 200);
			Animation walkDown = Animation(3, 3, 200);
			Animation shootSideways = Animation(4, 4, 50);
			Animation shootUp = Animation(6, 4, 50);
			Animation shootDown = Animation(5, 3, 50);

			animations.emplace("Idle", idle);
			animations.emplace("WalkSideways", walkSideways);
			animations.emplace("WalkUp", walkUp);
			animations.emplace("WalkDown", walkDown);
			animations.emplace("ShootSideways", shootSideways);
			animations.emplace("ShootUp", shootUp);
			animations.emplace("ShootDown", shootDown);
			play("Idle");
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
		TextureManager::draw(texture, srcRect, destRect,spriteFlip);
	}
	void play(const char* animationName)
	{
		frames = animations[animationName].frames;
		animationIndex = animations[animationName].index;
		speed = animations[animationName].speed;
	}
};