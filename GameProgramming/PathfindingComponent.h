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
	int timer = 0;
public:

	Vector2D destination;
	Entity* target;
	std::vector<Vector2D> pathPoints;


	PathfindingComponent() = default;

	~PathfindingComponent()
	{}

	PathfindingComponent(std::vector<Vector2D*> pts, std::vector<std::vector<int>> pa)
	{
		graph = new Graph(15);
		graph->points = pts;
		graph->paths = pa;
		destination = Vector2D(700.0f, 700.0f);
	}

	PathfindingComponent(std::vector<Vector2D*> &pts, std::vector<std::vector<int>> pa, Entity* mEntity)
	{
		graph->points = pts;
		graph->paths = pa;
		target = mEntity;
		destination = mEntity->getComponent<TransformComponent>().position;
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

	}
	
	void update() override
	{
		if (timer == 0) {
			findShortestPath();
			timer = 10;
		}
		else {
			timer--;
		}
		if (!pathPoints.empty()) {
			destination = pathPoints.front();
			goToDestination();
		}

		//do new path search after reaching a point or time x
	}

	void goToDestination() {
		int xdir = destination.x - transform->position.x;
		int ydir = destination.y - transform->position.y;
		if (!xdir == 0) {

			xdir = xdir / abs(xdir);
		}
		if (!ydir == 0) {
			ydir = ydir / abs(ydir);
		}
		transform->velocity.x = xdir;
		transform->velocity.y = ydir;
	}

	int findClosestPoint(Vector2D* position)
		// finds the closest point in points
		// returns the points id
	{
		int best;
		float score = INFINITY;
		for (int i = 0; i < graph->points.size(); i++) {
		//float distance = sqrt(pow(static_cast<float>(points[i]->x - transform->position.x), 2) +
		//			pow(static_cast<float>(points[i]->y - transform->position.y), 2));
		float distance = length(graph->points[i], position);
			if (distance < score) {
				best = i;
				score = distance;
			}
		}
		return best;
	}

	float length(Vector2D* start, Vector2D* end)
	{
		float euklid = sqrt(pow(static_cast<float>(start->x - end->x), 2) +
			pow(static_cast<float>(start->y - end->y), 2));
		return euklid;
	}

	void findShortestPath() {
		// add all edges to the graph
		for (int i = 0; i < graph->paths.size(); i++) {
			std::vector<int>* vec = &graph->paths[i];
			for (int j = 0; j < vec->size(); j++) {
				int distance = length(graph->points[i], graph->points[(*vec)[j]]);	//find euklid dist from 2 points of edge
				graph->addEdge(i, (*vec)[j], distance);
			}
		}
		int startID = findClosestPoint(&transform->position);	//find closest point to zombie in graph to start from
		int endID = findClosestPoint(&destination);

		pathPoints = graph->Dijkstra(startID, endID);
	}
};