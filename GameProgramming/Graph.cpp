#include "Graph.h"

// Create a graph with given vertices,
// and maintain an adjacency list
Graph::Graph(int vertices) {
    numVertices = vertices;
    adjLists = new list<int>[vertices];
}

// Add edges to the graph
void Graph::addEdge(int src, int dest, int weight) {
    adjLists[src].push_back(dest);
    adjLists[dest].push_back(src);
}

void Dijkstra(int startVertex)
{

};