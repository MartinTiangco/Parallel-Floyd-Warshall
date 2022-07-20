#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <list>
#include <stack>
using namespace std;

main(int argc, char *argv[])
{
    int numGraphs = 1;
    int order = 50;
    bool isRandom = false;
    int prob = 1;

    // 2nd arg is order
    if (argc >= 2)
    {
        order = atoi(argv[1]);
    }
    // 3rd arg is number of graphs (duplicates)
    if (argc >= 3)
    {
        numGraphs = atoi(argv[2]);
    }

    // 4th arg states the graph to be made is a random graph, with argv[4] being the
    // probability (%) of including a vertex into the neighbour list
    if (argc >= 4)
    {
        isRandom = true;
        prob = atoi(argv[3]);
    }

    // Create and open a text file
    ofstream MyFile("test-complete.txt");

    // Write a complete graph
    for (int i = 0; i < numGraphs; i++)
    {
        MyFile << order << "\n";
        for (int current = 0; current < order; current++)
        {
            vector<int> neighbours;
            // add all other vertex except the current vertex
            for (int vertex = 0; vertex < order; vertex++)
            {
                if (vertex != current)
                {
                    if (rand() % 100 > prob)
                        neighbours.push_back(vertex);
                }
            }

            for (int x : neighbours)
            {
                MyFile << x << " ";
            }
            MyFile << "\n";
        }
    }

    // Close the file
    MyFile << 0 << "\n";
    MyFile.close();
}