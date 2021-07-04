#pragma once
#include <iostream>
#include <list>
#include <vector>
#include "Vector2D.h"

using namespace std;

class Graph {
private:
	std::vector<std::vector<int> > graphWeights;
	int numVertices;
public:
	Graph(int vertices);
	void addEdge(int src, int dest, int weight);
	int minDistance(int dist[], bool sptSet[]);
	void Dijkstra(int startVertex, int target, std::vector<Vector2D> &result);
	std::vector<Vector2D*> points;
	std::vector<std::vector<int>> paths;

	void printPath(int parent[], int j);
	int printSolution(int dist[], int n,int parent[]);
};