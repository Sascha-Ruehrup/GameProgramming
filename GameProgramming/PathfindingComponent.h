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
	SpriteComponent* sprite;
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

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();

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
		int xdir = static_cast<int>(destination.x - transform->position.x);
		int ydir = static_cast<int>(destination.y - transform->position.y);
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
		if (xdir > 0) {
			sprite->spriteFlip = SDL_FLIP_NONE;
		}
		else if (xdir < 0) {
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		transform->velocity.x = static_cast<float>(xdir);
		transform->velocity.y = static_cast<float>(ydir);
		
	}

	int findClosestPoint(Vector2D* position)
		// finds the closest point in points
		// returns the points id
	{
		int best = 0;
		float score = INFINITY;
		for (int i = 0; i < graph->points.size(); i++) {
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
		float euklid = static_cast<float>(sqrt(pow(static_cast<float>(start->x - end->x), 2) +
			pow(static_cast<float>(start->y - end->y), 2)));
		return euklid;
	}

	void findShortestPath() {
		// add all edges to the graph
		for (int i = 0; i < graph->paths.size(); i++) {
			std::vector<int>* vec = &graph->paths[i];
			for (int j = 0; j < vec->size(); j++) {
				int distance = static_cast<int>(length(graph->points[i], graph->points[(*vec)[j]]));	//find euklid dist from 2 points of edge
				graph->addEdge(i, (*vec)[j], distance);
			}
		}
		int startID = findClosestPoint(&transform->position);	//find closest point to zombie in graph to start from
		int endID = findClosestPoint(&destination);

		graph->Dijkstra(startID, endID, pathPoints);
	}
};