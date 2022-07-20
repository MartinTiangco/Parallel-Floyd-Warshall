# About this assignment
This is an assignment for Computer Science 711 - Parallel and Distributed Computing. It instructs us to implement a graph algorithm that discovers the radius for a sequence of digraphs in C++. The algorithm is parallelized with OpenMP.

The radius of a graph is the minimum eccentricity of any vertex. The eccentricity is the maximum distance from a vertex v to any other vertex. 

The input is given in plaintext, as shown by the sample text files in the directory `sample-graphs`. The input sequence is as follows; the first line is the order n of the digraph (or the number of vertices). n lines follow, which lists the outgoing neighbours of the vertex. Vertices are numbered from 0 to n-1. This input is terminated by a digraph order of 0.

The corresponding output is an integer radius of the given digraph or 'None' if the digraph is not strongly connected.

##### Example:
Sample input:<br>
3<br>
1 2<br>
2<br>
0<br>
4<br>
2<br>
0<br>
0 3<br>
1<br>
5<br>
1 2<br>
2 0<br>
0 1<br>
4<br>
3<br>
0<br>

Sample output:<br>
1<br>
2<br>
None<br>