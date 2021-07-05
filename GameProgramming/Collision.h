#pragma once
#include "SDL.h"

class ColliderComponent;

struct Circle
{
	int x, y;
	int r;
};

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

	static bool CircleRect(const Circle& circleA, const SDL_Rect& recB);
	static bool CircleRect(const ColliderComponent& circleA, const ColliderComponent& recB);


};
