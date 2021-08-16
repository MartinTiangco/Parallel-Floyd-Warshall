#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define high_val 9999;
#define low_val -9999;

#include <iostream>
#include <list>
#include <stack>
using namespace std;

class Graph
{
    int V;          // No. of vertices
    list<int> *adj; // An array of adjacency lists

    // A recursive function to print DFS starting from v
    void DFSUtil(int v, bool visited[]);

public:
    // Constructor and Destructor
    Graph(int V)
    {
        this->V = V;
        adj = new list<int>[V];
    }
    ~Graph() { delete[] adj; }

    // Method to add an edge
    void addEdge(int v, int w);

    // The main function that returns true if the graph is strongly
    // connected, otherwise false
    bool isSC();

    // Function that returns reverse (or transpose) of this graph
    Graph getTranspose();
};

// A recursive function to print DFS starting from v
void Graph::DFSUtil(int v, bool visited[])
{
    // Mark the current node as visited and print it
    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited);
}

// Function that returns reverse (or transpose) of this graph
Graph Graph::getTranspose()
{
    Graph g(V);
    for (int v = 0; v < V; v++)
    {
        // Recur for all the vertices adjacent to this vertex
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
    adj[v].push_back(w); // Add w to v’s list.
}

// The main function that returns true if graph is strongly connected
bool Graph::isSC()
{
    // St1p 1: Mark all the vertices as not visited (For first DFS)
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Step 2: Do DFS traversal starting from first vertex.
    DFSUtil(0, visited);

    // If DFS traversal doesn’t visit all vertices, then return false.
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            return false;

    // Step 3: Create a reversed graph
    Graph gr = getTranspose();

    // Step 4: Mark all the vertices as not visited (For second DFS)
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Step 5: Do DFS for reversed graph starting from first vertex.
    // Staring Vertex must be same starting point of first DFS
    gr.DFSUtil(0, visited);

    // If all vertices are not visited in second DFS, then
    // return false
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            return false;

    return true;
}

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

    // print matrix
    // cout << "Initial Matrix \n";
    // for (int i = 0; i < order; i++)
    // {
    //     for (int j = 0; j < order; j++)
    //     {
    //         cout << matrix[i][j] << "      ";
    //     }
    //     cout << "\n";
    // }

    // print matrix
    // cout << "Floyd \n";
    // for (int i = 0; i < order; i++)
    // {
    //     for (int j = 0; j < order; j++)
    //     {
    //         cout << floydWarshall[i][j] << "      ";
    //     }
    //     cout << "\n";
    // }

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

int main(int argc, char *argv[])
{
    // there is a test file
    if (argc == 2)
    {
    }

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

        // check if graph is strongly connected
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

        if (graph.isSC())
        {
            // find radius
            result[numGraphs] = findRadius(matrix, order);
        }
        else
        {
            // result is "None"
            result[numGraphs] = 999;
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