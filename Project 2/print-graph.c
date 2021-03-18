/*
 Sarah Anderson
 ECE 4730: Parallel Systems
 Floyds Algor
 Due: Nove 7th, 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#include "graph.h"

int main (int argc, char *argv[]){
  int i, nodes, **graph;

  if (argc != 2){
    fprintf(stdout, "Program execution requires an input file argument.\n");
    exit(-1);
  }

  //reads in the graph
  read_graph(argv[1], &nodes, &graph);

  //print the graph to the console
  print_graph(nodes, graph);

  //frees the graph
  for (i = 0; i < nodes; i++){
    free(graph[i]); //frees columns
  }
  free(graph); //free rows

  return 0;
}
