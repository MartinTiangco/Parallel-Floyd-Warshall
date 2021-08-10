#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

// A structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
    struct AdjListNode *next;
};

// A structure to represent an adjacency list
struct AdjList
{
    struct AdjListNode *head;
};

// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices
// in graph)
struct Graph
{
    int numVertices;
    struct AdjList *array;
};

// A utility function to create a new adjacency list node
struct AdjListNode *newAdjListNode(int dest)
{
    struct AdjListNode *newNode =
        (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph *createGraph(int numVertices)
{
    struct Graph *graph =
        (struct Graph *)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;

    // Create an array of adjacency lists.  Size of
    // array will be numVertices
    graph->array =
        (struct AdjList *)malloc(numVertices * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by
    // making head as NULL
    int i;
    for (i = 0; i < numVertices; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Add edge to directed graph
void addEdge(struct Graph *graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    struct AdjListNode *newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

// A utility function to print the adjacency list
// representation of graph
void printGraph(struct Graph *graph)
{
    int v;
    for (v = 0; v < graph->numVertices; ++v)
    {
        struct AdjListNode *pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// int **performFloydWarshall()
// {
//     int **distanceMatrix;
//     return distanceMatrix;
// }

// void findRadius(struct Graph *graph)
// {
//     // Floyd Warshall on a adjacency matrix, output the shortest distance matrix

//     int radius = INT_MAX;
//     // for each vertex v
//     for (int v = 0; v < graph->numVertices; v++)
//     {
//         int eccentricity = INT_MIN;
//         // for each vertex u
//         for (int u = 0; u < graph->numVertices; u++)
//         {
//             eccentricity = max(eccentricity, d(v, u));
//         }
//         radius = min(radius, eccentricity);
//     }
// }

int main(int argc, char *argv[])
{
    char order[2];
    fgets(order, sizeof order, stdin); // read first order
    while (atoi(order) != 0)           // convert char to int
    {
        int orderOfGraph = atoi(order);

        // fgets recognises a \n character in the input, so we need to remove it
        // before our adjacency list
        getchar();

        // struct Graph *graph = createGraph(orderOfGraph);

        int matrix[orderOfGraph][orderOfGraph];
        for (int i = 0; i < orderOfGraph; i++)
        {
            for (int j = 0; j < orderOfGraph; j++)
            {
                matrix[i][j] = 0;
            }
        }

        // loop through each vertex
        for (int i = 0; i < orderOfGraph; i++)
        {
            char neighbourList[50];
            char *token;
            char sep[2] = " ";

            // get one line of neighbour vertices
            fgets(neighbourList, sizeof neighbourList, stdin);

            // first neighbour
            token = strtok(neighbourList, sep);
            // walk through other neighbours
            while (token != NULL)
            {
                // addEdge(graph, i, atoi(token));
                int indexOfNeighbour = atoi(token);
                matrix[i][indexOfNeighbour] = 1;

                // go to next token
                token = strtok(NULL, sep);
            }
        }

        // now we have a graph, we can perform this in parallel
        // findRadius(graph);
        // printGraph(graph);
        for (int i = 0; i < orderOfGraph; i++)
        {
            for (int j = 0; j < orderOfGraph; j++)
            {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        fgets(order, sizeof order, stdin);
    }

    // int nthreads,
    //     tid;
    // {
    //     tid = omp_get_thread_num();
    //     printf("welcome to GFG from thread = %d\n", tid);
    //     if (tid == 0)
    //     {
    //         nthreads = omp_get_num_threads();
    //         printf("number of threads = %d\n", nthreads);
    //     }
    // }

    return 0;
}