#pragma once
#include "SDL.h"
#include "Components.h"
#include <vector>

class PathfindingComponent : public Component
{
public:
	Vector2D destination;
	Entity* target;
	static std::vector<Vector2D> Points;
	std::vector<Vector2D> PathPoints;

	PathfindingComponent() = default;

	~PathfindingComponent()
	{}

	PathfindingComponent(Vector2D dest)
	{
		destination = dest;
	}

	PathfindingComponent(Entity* mEntity)
	{
		target = mEntity;
		destination = mEntity->getComponent<TransformComponent>().position;
	}

	void update() override
	{

	}
	void addPoint(Vector2D point)
	{
		Points.emplace_back(point);
	}

	void calculatePath(Vector2D dest)
	{

	}
};