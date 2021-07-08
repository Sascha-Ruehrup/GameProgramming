#include "Graph.h"
#include <algorithm>

#define V 15    // hardcoded for specific Mapfile

// Create a graph with given vertices,
// and maintain an adjacency list
Graph::Graph(int vertices) {
    numVertices = vertices;
    //graphWeights(vertices, vector<int>(vertices, 0)); // fill with
    for (int i = 0; i < vertices; i++)
    {
        std::vector <int> vec0;
        for (int j = 0; j < vertices; j++)
        {
            vec0.push_back(0);
        }
        graphWeights.push_back(vec0);
    }
}

// Add edges to the graph
void Graph::addEdge(int src, int dest, int weight) {
    vector<int>* vec = &graphWeights[src];
    (*vec)[dest] = weight;
}


// A utility function to find the 
// vertex with minimum distance
// value, from the set of vertices
// not yet included in shortest
// path tree
int Graph::minDistance(int dist[],bool sptSet[])
{

    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false &&
            dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}


// Funtion that implements Dijkstra's
// single source shortest path
// algorithm for a graph represented
// using adjacency matrix representation
 void Graph::Dijkstra(int startVertex, int target, std::vector<Vector2D> &result)
{
    //std::vector<Vector2D> result;
    // std::vector<Vector2D>
    // 
    // The output array. dist[i]
    // will hold the shortest
    // distance from src to i
    int dist[V];

    // sptSet[i] will true if vertex
    // i is included / in shortest
    // path tree or shortest distance 
    // from src to i is finalized
    bool sptSet[V];

    // Parent array to store
    // shortest path tree
    int parent[V];

    // Initialize all distances as 
    // INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
    {
        parent[i] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    // Distance of source vertex 
    // from itself is always 0
    dist[startVertex] = 0;

    // Find shortest path
    // for all vertices
    for (int count = 0; count < V - 1; count++)
    {
        // Pick the minimum distance
        // vertex from the set of
        // vertices not yet processed. 
        // u is always equal to src
        // in first iteration.
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex 
        // as processed
        sptSet[u] = true;

        if (u == target) {
            if ((parent[u] != -1) || (u = startVertex)) {
                while (u != -1) {
                    result.push_back((*points[u]));
                    u = parent[u];
                }
            }
            //return result;
            return;
        }


        // Update dist value of the 
        // adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)

            // Update dist[v] only if is
            // not in sptSet, there is
            // an edge from u to v, and 
            // total weight of path from
            // src to v through u is smaller
            // than current value of
            // dist[v]
            if (!sptSet[v] && graphWeights[u][v] &&
                dist[u] + graphWeights[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graphWeights[u][v];
            }
    }

    return;
}