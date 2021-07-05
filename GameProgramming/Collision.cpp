#include "Collision.h"
#include "ColliderComponent.h"
bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}

	return false;
}
bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (AABB(colA.collider, colB.collider))
	{
		std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision::CircleRect(const Circle& cirA, const SDL_Rect& recB)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (cirA.x < recB.x)
	{
		cX = recB.x;
	}
	else if (cirA.x > recB.x + recB.w)
	{
		cX = recB.x + recB.w;
	}
	else
	{
		cX = cirA.x;
	}

	//Find closest y offset
	if (cirA.y < recB.y)
	{
		cY = recB.y;
	}
	else if (cirA.y > recB.y + recB.h)
	{
		cY = recB.y + recB.h;
	}
	else
	{
		cY = cirA.y;
	}

	//If the closest point is inside the circle
	if (distanceSquared(cirA.x, cirA.y, cX, cY) < cirA.r * cirA.r)
	{
		//This box and the circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;
}
double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}
	
bool Collision::CircleRect(const ColliderComponent& circleA, const ColliderComponent& recB) {
	if (CircleRect(circleA.colliderCircle, recB.collider))
	{
		std::cout << circleA.tag << " hit: " << recB.tag << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}