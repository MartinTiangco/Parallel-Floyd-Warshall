#include <iostream>
#include <bits/stdc++.h>
#include <list>
#include <stack>
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
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // DFS starting from first vertex.
    DFS(0, visited);

    // If DFS doesn’t visit all vertices, return false.
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            return false;

    // Reverse the graph and repeat the steps performed before
    Graph gr = getTranspose();

    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Perform DFS from the same vertex as the first DFS
    gr.DFS(0, visited);

    // If DFS doesn't visit all vertices in reverse graph, return false
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            return false;

    return true;
}

// Uses Floyd Warshall's Algorithm to find the radius of the graph.
int findRadius(int **matrix, int order)
{
    int floydWarshall[order][order];

    // init with adj matrix values
    for (int i = 0; i < order; i++)
        for (int j = 0; j < order; j++)
            floydWarshall[i][j] = matrix[i][j];

    // find shortest distance using intermediate node k
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
                    floydWarshall[i][j] = floydWarshall[i][k] + floydWarshall[k][j];
            }
        }
    }

    // Source: https://stackoverflow.com/questions/30592245/finding-radius-of-graph
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

    // for single node graph
    if (radius == 0)
    {
        radius = -1;
    }

    return radius;
}

int main()
{
    int result[256];
    int numGraphs = 0;

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
        for (int i = 0; i < order; i++)
            matrix[i] = new int[order];

        // init matrix
        // A vertex v to itself has 0 distance
        // Otherwise, a large number representing infinite distance
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
        for (int i = 0; i < order; i++)
            delete[] matrix[i];
        delete[] matrix;

        numGraphs++;
    }

    // print results
    for (int i = 0; i < numGraphs; i++)
    {
        if (result[i] == 999 || result[i] == -1)
            printf("None\n");
        else
            printf("%d\n", result[i]);
    }

    return 0;
}