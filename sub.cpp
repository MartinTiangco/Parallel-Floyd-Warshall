#include <iostream>
#include <bits/stdc++.h>
#include <list>
#include <stack>
#include <omp.h>
using namespace std;

#define high_val 9999;
#define low_val -9999;

// Adapted from https://www.geeksforgeeks.org/connectivity-in-a-directed-graph/
class Graph
{
    int V;          // No. of vertices
    list<int> *adj; // An array of adjacency lists

    // DFS starting from vertex v
    void DFS(int v, bool visited[]);

public:
    Graph(int V)
    {
        this->V = V;
        adj = new list<int>[V];
    }
    ~Graph() { delete[] adj; }

    void addEdge(int v, int w);

    // If graph is strongly connected, return true. False otherwise.
    bool isStronglyConnected();

    // Returns reverse of the graph
    Graph getTranspose();
};

// DFS starting from vertex v
void Graph::DFS(int v, bool visited[])
{
    // Current vertex is visited
    visited[v] = true;

    // Recursion for adj vertices
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFS(*i, visited);
}

Graph Graph::getTranspose()
{
    Graph g(V);
    for (int v = 0; v < V; v++)
    {
        // Recursion for adj vertices
        list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            g.adj[*i].push_back(v);
        }
    }
    return g;
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

bool Graph::isStronglyConnected()
{
    // init visited values to be false
    bool visited[V];
    bool reverseVisited[V];
    bool isStronglyConnected = true;

    // OpenMP section trialled here, but has no noticeable effect on performance. I removed
    // them due to added overhead.
    // #pragma omp sections
    {

        // #pragma omp section
        {
            for (int i = 0; i < V; i++)
                visited[i] = false;

            // DFS starting from first vertex.
            DFS(0, visited);

            // If DFS doesn’t visit all vertices, return false.
            for (int i = 0; i < V; i++)
                if (visited[i] == false)
                    // return false;
                    isStronglyConnected = false;
        }

        // #pragma omp section
        {
            // Reverse the graph and repeat the steps performed before
            Graph gr = getTranspose();

            for (int i = 0; i < V; i++)
                reverseVisited[i] = false;

            // Perform DFS from the same vertex as the first DFS
            gr.DFS(0, reverseVisited);

            // If DFS doesn't visit all vertices in reverse graph, return false
            for (int i = 0; i < V; i++)
                if (reverseVisited[i] == false)
                    isStronglyConnected = false;
        }

        return isStronglyConnected;
    }
}

// Uses Floyd Warshall's Algorithm to find the radius of the graph.
int findRadius(int **matrix, int order)
{
    // declare matrix
    int **floydWarshall;
    floydWarshall = new int *[order];
    for (int i = 0; i < order; i++)
        floydWarshall[i] = new int[order];

// init with adj matrix values
#pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < order; i++)
        for (int j = 0; j < order; j++)
            floydWarshall[i][j] = matrix[i][j];

// find shortest distance using intermediate node k
#pragma omp parallel for schedule(dynamic, 5)
    for (int k = 0; k < order; k++)
    {
        // source vertex i
        for (int i = 0; i < order; i++)
        {
            // dest vertex j
            for (int j = 0; j < order; j++)
            {
                // update dist if path through intermediate vertex k
                // is shorter
                if (floydWarshall[i][k] + floydWarshall[k][j] < floydWarshall[i][j])
#pragma omp critical
                    floydWarshall[i][j] = floydWarshall[i][k] + floydWarshall[k][j];
            }
        }
    }

    int radius = high_val;
    // for each vertex v
    for (int v = 0; v < order; v++)
    {
        int eccentricity = low_val;
        // for each vertex u
        for (int u = 0; u < order; u++)
            eccentricity = max(eccentricity, floydWarshall[v][u]);

        radius = min(radius, eccentricity);
    }

// delete matrix
#pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < order; i++)
        delete[] floydWarshall[i];
    delete[] floydWarshall;

    return radius;
}

int main(int argc, char *argv[])
{

    int result[256];
    int numGraphs = 0;
    int numThreads = omp_get_num_threads();

    // for timing
    double start = omp_get_wtime();

    while (true)
    {
        // get order
        string orderString;
        getline(cin, orderString);
        int order = stoi(orderString);

        // cout << order << "\n";

        // exit loop
        if (order == 0)
        {
            break;
        }

        // declare matrix
        int **matrix;
        matrix = new int *[order];
#pragma omp parallel for schedule(static, 3)
        for (int i = 0; i < order; i++)
            matrix[i] = new int[order];

// init matrix
// A vertex v to itself has 0 distance
// Otherwise, a large number representing infinite distance
#pragma omp parallel for schedule(static, 3)
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                if (i == j)
                    matrix[i][j] = 0;
                else
                    matrix[i][j] = 999;
            }
        }

        // get neighbours
        // Source: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
        for (int currentNode = 0; currentNode < order; currentNode++)
        {
            string neighbourLine;
            getline(cin, neighbourLine);

            // cout << "neighbours: " << neighbourLine << "\n";
            vector<string> tokens;
            stringstream check1(neighbourLine);
            string intermediate;
            while (getline(check1, intermediate, ' '))
            {
                tokens.push_back(intermediate);
            }

            for (int j = 0; j < tokens.size(); j++)
            {
                // cout << "token at j=" << j << tokens[j] << "\n";
                // add edge
                int indexOfNeighbour = stoi(tokens[j]);
                matrix[currentNode][indexOfNeighbour] = 1;
            }
        }

        // first check if graph is strongly connected
        Graph graph(order);
#pragma omp parallel for schedule(static, 3)
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                if (!(matrix[i][j] == 0 || matrix[i][j] == 999))
                {
                    graph.addEdge(i, j);
                }
            }
        }

        if (graph.isStronglyConnected())
        {
            // find radius
            result[numGraphs] = findRadius(matrix, order);
        }
        else
        {
            // result is "None" for non-strongly connected graphs
            result[numGraphs] = -1;
        }

// delete matrix
#pragma omp parallel for schedule(static, 3)
        for (int i = 0; i < order; i++)
            delete[] matrix[i];
        delete[] matrix;

        numGraphs++;
    }

    // print results
    for (int i = 0; i < numGraphs; i++)
    {
        if (result[i] == -1)
            cout << "None\n";
        else
            cout << result[i] << "\n";
    }

    // end timing
    double end = omp_get_wtime();

    // uncomment for timing
    // cout << (end - start) << "\n";

    return 0;
}