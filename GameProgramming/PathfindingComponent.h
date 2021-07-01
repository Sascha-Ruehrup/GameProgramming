#pragma once
#include "SDL.h"
#include "Components.h"
#include <vector>

#include <cmath>

class PathfindingComponent : public Component
{
private:
	TransformComponent* transform;
public:

	Vector2D destination;
	Entity* target;
	std::vector<std::vector<int>> paths;
	std::vector<Vector2D*> points;
	std::vector<Vector2D> pathPoints;


	PathfindingComponent() = default;

	~PathfindingComponent()
	{}

	PathfindingComponent(std::vector<Vector2D*> pts, std::vector<std::vector<int>> pa)
	{
		points = pts;
		paths = pa;
	}

	PathfindingComponent(std::vector<Vector2D*> &pts, std::vector<std::vector<int>> pa, Entity* mEntity)
	{
		points = pts;
		paths = pa;
		target = mEntity;
		destination = mEntity->getComponent<TransformComponent>().position;
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	
	void update() override
	{
		/*int xdir = points[14]->x - transform->position.x;
		int ydir = points[14]->y - transform->position.y;
		if (!xdir == 0) {

			xdir = xdir / abs(xdir);
		}
		if (!ydir == 0) {
			ydir = ydir / abs(ydir);
		}
		transform->velocity.x = xdir;
		transform->velocity.y = ydir;*/
	}

	void calculatePath(Vector2D dest)
	{

	}
	void breathsearch() {

	}
};