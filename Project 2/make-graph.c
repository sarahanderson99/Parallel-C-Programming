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

/*
 *  Processes command line arguments
 *  make-graph accepts four arguments:
 *  -n : the number of nodes
 *  -r : maximum edge weight
 *  -p : maximum probablity
 *  -o : the name of the output file
 */
void ProcessCommandLine(int argc, char *argv[], int *nodes, int *weight, int *probablity, char *ofile[]){
    int c;
    while ((c = getopt(argc, argv, "n:r:p:o:")) != -1){
      switch(c){
          case 'n':
            *nodes = atoi(optarg);
            break;
          case 'r':
            *weight = atoi(optarg);
            break;
          case 'p':
            *probablity = atoi(optarg);
            break;
          case 'o':
            if (strcmp(optarg, "default-make-graph-file.dat") != 0){
              *ofile = strdup(optarg);
            }
            break;
      }
    }
}

int main (int argc, char *argv[]){
  int i, j, u;
  int nodes = 5, weight = 100, probablity = 150;
  int **graph = NULL;
  char *output_file = "default-make-graph-file.dat";
  time_t t;

  //parse command line arguments
  ProcessCommandLine(argc, argv, &nodes, &weight, &probablity, &output_file);

  //allocate for graph (n x n matrix)
  graph = (int **) malloc(nodes * sizeof(int *)); //rows
  for (i = 0; i < nodes; i++){
    graph[i] = (int *) malloc(nodes * sizeof(int)); //cols
  }

  //seeds a random number generator
  srand(time(&t));

  //calculates the edges
  for (i = 0; i < nodes; i++){
    for (j = 0; j < nodes; j++){
      u = rand() % probablity;

      if (i == j){
        graph[i][j] = 0;
      }
      else if (u <= weight){
        graph[i][j] = u;
      }
      else{
        graph[i][j] = -1;
      }
    }
  }

  //write graph to output file
  write_graph_serial(output_file, nodes, graph);

  //free output file pointer from call to strdup
  if (strcmp(output_file, "default-make-graph-file.dat") != 0){
    free(output_file);
  }

  //free graph
  for (i = 0; i < nodes; i++){
    free(graph[i]); //frees columns
  }
  free(graph); //free rows

  return 0;
}
