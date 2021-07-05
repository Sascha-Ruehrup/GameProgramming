#pragma once
#include "SDL.h"
#include "Components.h"
#include <vector>
#include "Graph.h"
#include <cmath>
#include "Game.hpp"

class PathfindingComponent : public Component
{
private:
	TransformComponent* transform;
	Graph* graph;
	int timer = 0;
public:

	Vector2D destination;
	Vector2D latest;
	bool targetPlayer = true;
	std::vector<Vector2D> pathPoints;


	PathfindingComponent() = default;

	~PathfindingComponent()
	{}

	PathfindingComponent(std::vector<Vector2D*> pts, std::vector<std::vector<int>> pa)
	{
		graph = new Graph(pts.size());
		graph->points = pts;
		graph->paths = pa;
		destination = *Game::playerPosition;
	}

	/*PathfindingComponent(std::vector<Vector2D*> &pts, std::vector<std::vector<int>> pa)
	{
		graph->points = pts;
		graph->paths = pa;

	}*/

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

	}
	
	void update() override
	{
		//findShortestPath();
		if (timer == 0) {
			findShortestPath();
			timer = 10;
		}
		else {
			timer--;
		}
		if (!pathPoints.empty()) {
			destination = pathPoints.back();
			if (destination == latest) {
				pathPoints.pop_back();
			}if (!pathPoints.empty()) {
				if (pathPoints.back() == transform->position) {
					latest = pathPoints.back();
					pathPoints.pop_back();
					std::cout << "popped pathpoints back!" << std::endl;
				}
			}else {
				if(targetPlayer){
					destination = *Game::playerPosition;
				}

			}
			
		}

		goToDestination();
	}

	void goToDestination() {
		int xdir = destination.x - transform->position.x;
		int ydir = destination.y - transform->position.y;
		if ((xdir < 5) && (ydir<5)) {
			transform->speed = 1;
		}else {
			transform->speed = 3;
		}
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

		graph->Dijkstra(startID, endID, pathPoints);
	}
};