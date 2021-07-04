#include "Graph.h"
#include <algorithm>

#define V 15

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

/*std::vector<Vector2D> Graph::Dijkstra(int startVertex, int target)
{
    std::vector<int> Q;
    std::vector<int> dist;
    std::vector<int> prev;
    std::vector<bool> visited;

    std::vector<Vector2D> shortestPath;

    for (int v = 0; v < paths.size(); v++)
    {
        dist.push_back(INT_MAX);
        prev.push_back(-1);
        Q.push_back(v);
        visited.push_back(false);
    }
    dist[startVertex] = 0;

    while (!Q.empty())
    {
        int lowestVal = INT_MAX;
        int u = -1;
        for (int i = 0; i < dist.size(); i++) {
            if (dist[i]<lowestVal){
                lowestVal = dist[i];
                u = i;
            }
        }

        


        // check if u is target
        if (u == target) {
            if ((prev[u]!=-1) || (u = startVertex)) {
                while (u != -1) {
                    shortestPath.push_back(*points[u]);
                    u = prev[u];
                }
            }
        }

        // extend to all neighbors
        for (int index : graphWeights[u]) {
            if (graphWeights[u][index] != 0) {
                int length = graphWeights[u][index];
                int alt = dist[u] + length;
                if (alt < dist[index]) {
                    dist[index] = alt;
                    prev[index] = u;
                }
            }
        }
       
    }
    return shortestPath;
};*/



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

// Function to print shortest
// path from source to j
// using parent array
void Graph::printPath(int parent[], int j)
{

    // Base Case : If j is source
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j]);

    printf("%d ", j);
}

// A utility function to print 
// the constructed distance
// array
int Graph::printSolution(int dist[], int n,int parent[])
{
    int src = 0;
    printf("Vertex\t Distance\tPath");
    for (int i = 1; i < V; i++)
    {
        printf("\n%d -> %d \t\t %d\t\t%d ",
            src, i, dist[i], src);
        printPath(parent, i);
    }
    return 0;
}

// Funtion that implements Dijkstra's
// single source shortest path
// algorithm for a graph represented
// using adjacency matrix representation
std::vector<Vector2D> Graph::Dijkstra(int startVertex, int target)
{
    std::vector<Vector2D> result;
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

    // print the constructed
    // distance array
    printSolution(dist, V, parent);

    return result;
}