/*
Sarah Anderson
ECE 4730: Parallel Systems
Floyds Algor
Due: Nove 7th, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include "graph.h"

//A serial implementation of Floyd's Algorithm
void floyd_serial(int nodes, int ***graph){
  int src, dest, k;

  for (k = 0; k < nodes; k++){
    // Pick all vertices as source one by one
    for (src = 0; src < nodes; src++){
      // Pick all vertices as destination for the
      for (dest = 0; dest < nodes; dest++){
          if(
                (IsEdge((*graph)[src][k], (*graph)[k][dest]) && ((*graph)[src][dest] == -1))
                                                             ||
                (IsEdge((*graph)[src][k], (*graph)[k][dest]) && (((*graph)[src][k] + (*graph)[k][dest]) < (*graph)[src][dest]))
              )
                (*graph)[src][dest] = (*graph)[src][k] + (*graph)[k][dest];
      }
    }
  }
}

int main (int argc, char *argv[]){
  int i, nodes, **graph;
  clock_t prog_exec_time = -clock();
  clock_t floyd_exec_time;

  //process command line arguments
  if (argc != 3){
    fprintf(stdout, "Program execution requires input and output file arguments.\n");
    exit(-1);
  }

  //read in the graph
  read_graph(argv[1], &nodes, &graph);

  //calculate shortest paths
  floyd_exec_time = -clock();
  floyd_serial(nodes, &graph);
  floyd_exec_time += clock();

  //write shortest path graph to output file
  write_graph_serial(argv[2], nodes, graph);

  //free graph
  for (i = 0; i < nodes; i++){
    free(graph[i]);
  }
  free(graph);

  //output timing results
  prog_exec_time += clock();
  fprintf(stdout, "floyd-parallel execution time:\n");
  fprintf(stdout, "\tn = %5d nodes\n", nodes);
  fprintf(stdout, "\tp = %5d cpus\n", 1);
  fprintf(stdout, "\tptime = %6.5f (sec)\n", ((double)prog_exec_time)/CLOCKS_PER_SEC);
  fprintf(stdout, "\tftime = %6.5f (sec)\n", ((double)floyd_exec_time)/CLOCKS_PER_SEC);

  return 0;
}
