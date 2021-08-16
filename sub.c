#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define INF 999
#define lowest_val -999
#define highest_val 999
#define max_int(x, y) (((x) >= (y)) ? (x) : (y))
#define min_int(x, y) (((x) <= (y)) ? (x) : (y))

int findRadius(int **matrix, int order)
{
    // This is the output matrix
    // with the shortest distances between every vertex pair
    // int dist[order][order];
    // allocate matrix
    int **dist;
    dist = malloc(order & sizeof *dist);
    for (int i = 0; i < order; i++)
        dist[i] = malloc(order * sizeof *dist[i]);

    // init with adj matrix values
    for (int i = 0; i < order; i++)
        for (int j = 0; j < order; j++)
            dist[i][j] = matrix[i][j];

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
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    int radius = highest_val;
    // for each vertex v
    for (int v = 0; v < order; v++)
    {
        int eccentricity = lowest_val;
        // for each vertex u
        for (int u = 0; u < order; u++)
            eccentricity = max_int(eccentricity, dist[v][u]);

        radius = min_int(radius, eccentricity);
    }

    /* deallocate the array */
    for (int i = 0; i < order; i++)
        free(dist[i]);
    free(dist);

    return radius;
}

int main(int argc, char *argv[])
{
    int output[255];
    int numGraphs = 0;
    char order[2];
    fgets(order, sizeof order, stdin); // read first order
    while (atoi(order) != 0)           // convert char to int
    {
        int orderOfGraph = atoi(order);

        // fgets recognises a \n character in the input, so we need to remove it
        // before our adjacency list
        getchar();

        // allocate matrix
        int **matrix;
        matrix = malloc(orderOfGraph & sizeof *matrix);
        for (int i = 0; i < orderOfGraph; i++)
            matrix[i] = malloc(orderOfGraph * sizeof *matrix[i]);

        // init matrix
        // A vertex v to itself has 0 distance
        // Otherwise, a large number representing infinite distance
        for (int i = 0; i < orderOfGraph; i++)
        {
            for (int j = 0; j < orderOfGraph; j++)
            {
                if (i == j)
                    matrix[i][j] = 0;
                else
                    matrix[i][j] = INF;
            }
        }

        // loop through each vertex
        for (int i = 0; i < orderOfGraph; i++)
        {
            char neighbourList[30];
            char *token;
            char sep[2] = " ";

            // get one line of neighbour vertices
            fgets(neighbourList, sizeof neighbourList, stdin);

            // first neighbour
            token = strtok(neighbourList, sep);
            // walk through other neighbours
            while (token != NULL)
            {
                // add edge to adjacency matrix
                int indexOfNeighbour = atoi(token);
                matrix[i][indexOfNeighbour] = 1;

                // go to next token
                token = strtok(NULL, sep);
            }
        }

        // now we have a graph, we can perform this in parallel
        int radius = findRadius(matrix, orderOfGraph);
        output[numGraphs] = radius;

        // free memory
        for (int i = 0; i < orderOfGraph; i++)
            free(matrix[i]);
        free(matrix);

        numGraphs++;

        // read next digraph order
        fgets(order, sizeof order, stdin);
    }

    // print results
    for (int i = 0; i < numGraphs; i++)
    {
        if (output[i] == 999)
            printf("None\n");
        else
            printf("%d\n", output[i]);
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