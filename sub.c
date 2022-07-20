#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define INF 999
#define lowest_val -999
#define highest_val 999
#define max_int(x, y) (((x) >= (y)) ? (x) : (y))
#define min_int(x, y) (((x) <= (y)) ? (x) : (y))

typedef enum
{
    false = 0,
    true
} bool;

bool isEmptyLine(const char *s)
{
    static const char *emptyline_detector = " \t\n";

    return strspn(s, emptyline_detector) == strlen(s);
}

// Floyd Warshall: https://www.geeksforgeeks.org/floyd-warshall-algorithm-dp-16/
int findRadius(int **matrix, int order)
{
    // This is the output matrix
    // with the shortest distances between every vertex pair
    // allocate matrix
    int **dist;
    dist = malloc(order * sizeof *dist);
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

    // Source: https://stackoverflow.com/questions/30592245/finding-radius-of-graph
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

    // printf("dist: \n");
    // for (int i = 0; i < order; i++)
    // {
    //     for (int j = 0; j < order; j++)
    //     {
    //         printf("%d   ", dist[i][j]);
    //     }
    //     printf("\n");
    // }

    /* deallocate the array */
    for (int i = 0; i < order; i++)
        free(dist[i]);
    free(dist);

    return radius;
}

int main(int argc, char *argv[])
{
    // there is a test file
    if (argc == 2)
    {
        // extract the test file
        freopen(argv[1], "r", stdin);
    }

    // allocate
    int *output;
    output = malloc(512 * sizeof(int));
    for (int i = 0; i < 512; i++)
        output[i] = 0;

    int numGraphs = 0;
    char order[10];
    fgets(order, sizeof order, stdin); // read first order
    while (atoi(order) != 0)           // convert char to int
    {
        int orderOfGraph = atoi(order);
        // printf("Order: %d\n", orderOfGraph);

        // fgets recognises a \n character in the input, so we need to remove it
        // before our adjacency list
        // getchar();

        // allocate matrix
        int **matrix;
        matrix = malloc(orderOfGraph * sizeof *matrix);
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

        // printf("init:\n");
        // for (int i = 0; i < orderOfGraph; i++)
        // {
        //     for (int j = 0; j < orderOfGraph; j++)
        //     {
        //         printf("%d   ", matrix[i][j]);
        //     }
        //     printf("\n");
        // }

        // loop through each vertex
        for (int i = 0; i < orderOfGraph; i++)
        {
            char neighbourList[512];
            char *token;
            char sep[2] = " ";

            // get one line of neighbour vertices
            fgets(neighbourList, sizeof neighbourList, stdin);

            // empty if neighbourList string length is 1
            // source: https://www.ibm.com/docs/en/i/7.4?topic=functions-fgets-read-string
            if (strlen(neighbourList) != 1)
            {
                // first neighbour
                token = strtok(neighbourList, sep);
                // walk through other neighbours
                while (token != NULL)
                {
                    // printf("Neighbour of %d: %d\n", i, atoi(token));
                    // add edge to adjacency matrix
                    int indexOfNeighbour = atoi(token);
                    matrix[i][indexOfNeighbour] = 1;

                    // go to next token
                    token = strtok(NULL, sep);
                }
            }

            // for (int i = 0; i < strlen(neighbourList); i++)
            // {
            //     printf("%d: %c ", i, neighbourList[i]);
            // }
            // printf("\n");
        }

        // printf("with neighbours:\n");
        // for (int i = 0; i < orderOfGraph; i++)
        // {
        //     for (int j = 0; j < orderOfGraph; j++)
        //     {
        //         printf("%d   ", matrix[i][j]);
        //     }
        //     printf("\n");
        // }

        // now we have a graph, we can perform this in parallel
        output[numGraphs] = findRadius(matrix, orderOfGraph);
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

    free(output);

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