#pragma once
#include "SDL.h"
#include "Components.h"
#include <vector>
#include "Graph.h"
#include <cmath>

class PathfindingComponent : public Component
{
private:
	TransformComponent* transform;
	Graph* graph;
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

		//do new path search after reaching a point or time x


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
	int findClosestPoint()
		// finds the closest point to the zombie in points
		// returns the points id, can be used to start the search from
	{
		int best;
		float score = INFINITY;
		transform->position;
		for (int i = 0; i < points.size(); i++) {
			float euklid = sqrt(pow(static_cast<float>(points[i]->x - transform->position.x), 2) +
						pow(static_cast<float>(points[i]->y - transform->position.y), 2));
			if (euklid < score) {
				best = i;
				score = euklid;
			}
		}
		return best;
		
	}
	void breathsearch() {
		// add all edges to the graph
		for (int i = 0; i < paths.size(); i++) {
			std::vector<int>* vec = &paths[i];
			for (int j = 0; j < vec->size(); j++) {
				graph->addEdge(j, vec->at(j), 1);
			}
		}
		int startID = findClosestPoint();
		graph->Dijkstra(startID);

	}
};