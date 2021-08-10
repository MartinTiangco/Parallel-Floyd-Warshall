#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

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
                int indexOfNeighbour = atoi(token);
                matrix[i][indexOfNeighbour] = 1;

                // go to next token
                token = strtok(NULL, sep);
            }
        }

        // now we have a graph, we can perform this in parallel
        // findRadius(graph);
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