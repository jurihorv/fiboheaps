# Fibonacci heap applied on Dijkstra algorithm 
## (Algorithms & Data structures project) 

This approach to Dijkstra algorithm (shortest path from single source) is useful in cases of very dense graphs. Data structure in this project implements all the functionalities from Cormen's book: "Introduction to Algorithms". Code is written in C++(17). Last few slides of presentation compare performances between Dijkstra implemented with binary heaps and Fibonacci heaps on specific types of graph of interest.   

# Input/Output format
First line of input contains space separated integers V and E: the number of nodes and number of edges in graph, respectively. Next E lines contain 2 positive integers A and B (1 <= A, B <= V) and non-negative integer C: they denote an edge between vertices A and B of weight C. The last line of input contains integer S: source vertex. <br/>
Output is a list of V - 1 integers which are shortest distances from S to all other vertices from 1 to V, except S.

# Testing (Linux)
File fibodijk.cpp implements Dijsktra algorithm with Fibonacci heaps and dijkstra.cpp uses binary heaps (std::set). <br/> <br/>
1.) Compile codes `fibodijk.cpp` and/or `dijkstra.cpp` using g++ (std at least 17) <br/>
2.) Run program in terminal with `./fibodijk` (or `./dijkstra`)  <br/>
3.) Run program on large input (e.g. in.txt contains description of graph): `./fibodijk < in.txt > out.txt` <br/>
4.) Track running time of program: `time ./fibodijk < int.txt > out.txt` <br/>
Testing on Windows OS is very similar. <br/>
