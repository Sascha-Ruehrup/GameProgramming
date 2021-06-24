#pragma once

#include "Components.h"
#include "SDL.h"
#include "Texturemanager.h"
#include "Animation.h"
#include <map>

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
	SpriteComponent(const char* path)
	{
		setTex(path);
	}
	SpriteComponent(const char* path, bool isAnimated)
	{
		animated = isAnimated;
		Animation idle = Animation(1, 4, 200);	// row, spritecount, ms between sprites
		Animation walkSideways = Animation(2, 10, 50);
		Animation walkUp = Animation(0, 4, 200);

		animations.emplace("Idle", idle);
		animations.emplace("WalkSideways", walkSideways);
		animations.emplace("WalkUp", walkUp);
		animations.emplace("WalkDown", walkSideways); // TODO
		play("Idle");
		setTex(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
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