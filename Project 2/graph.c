/*
Sarah Anderson
ECE 4730: Parallel Systems
Floyds Algor
Due: Nove 7th, 2020
*/

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <mpi.h>
 #include "graph.h"
 #include "MyMPI.h"

//Verifies there is an edge between two vertices
int IsEdge(int vertex_a, int vertex_b){
   if (vertex_a == -1 || vertex_b == -1){
     return 0;
   }
   else{
     return 1;
   }
}


//Reads in a 2D graph from a file
void read_graph(char *file_name, int *n, int ***A){
  int i, j, nodes, **graph = NULL;
  FILE *ifptr;

  //let user know graph is being read
  fprintf(stdout, "reading graph from file %s\n", file_name);

  //open input file for reading
  ifptr = fopen(file_name, "rb");
  if (ifptr == NULL){
    fprintf(stderr, "Could not open %s\n", file_name);
    exit(-1);
  }

  //get the number of nodes in the graph
  fread(&nodes, sizeof(int), 1, ifptr);

  //allocate for graph (nodes x nodes matrix)
  graph = (int **) malloc(nodes * sizeof(int *)); //rows
  for (i = 0; i < nodes; i++){
    graph[i] = (int *) malloc(nodes * sizeof(int)); //cols
  }

  //read file into array
  for (i = 0; i < nodes; i++){
    for (j = 0; j < nodes; j++){
      fread(&graph[i][j], sizeof(int), 1, ifptr);
    }
  }
  fclose(ifptr);

  //return array and number of nodes
  *n = nodes, *A = graph;
}

//Writes a 2D graph to a file
void write_graph_serial(char *file_name, int n, int **A){
  int i, j;

  //open output file for writing
  FILE *ofptr = fopen(file_name, "wb");
  if (ofptr == NULL){
    fprintf(stderr, "Could not open %s\n", file_name);
    exit(-1);
  }

  //let user know graph is being written to file
  fprintf(stdout, "writing graph to file %s\n", file_name);

  //generate output file with list of integers
  fwrite(&n, sizeof(int), 1, ofptr);
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      fwrite(&A[i][j], sizeof(int), 1, ofptr);
    }
  }
  fclose(ofptr);
}

//Writes a process's portion of a graph to a file. The process's
//portion was determined using a 2D checkerboard matrix fashion.
void write_graph_parallel(char *file_name, int rank, int n, void **A, MPI_Comm grid_comm){
    
  if (rank == ROOT){
    fprintf(stdout, "writing graph to file %s\n", file_name);

    FILE *ofptr = fopen(file_name, "wb");
    if (ofptr == NULL){
      fprintf(stderr, "Could not open %s\n", file_name);
      exit(-1);
    }
    fwrite(&n, sizeof(int), 1, ofptr);
    fclose(ofptr);
  }
  write_checkerboard_matrix (file_name, (void **) A, MPI_INT, n, n, grid_comm);
}

//Prints a 2D matrix to the terminal
void print_graph(int n, int **A){
  int i, row, col;

  //let user know the size of the graph
  fprintf(stdout, "Array is a %d x %d matrix\n\n", n, n);

  //print header row with columns
  fprintf(stdout, "%5s", "|");
  for (i = 0; i < n; i++){
    fprintf(stdout, "%5d", i);
  }
  fprintf(stdout, "\n");

  //print row to separate graph from header row
  fprintf(stdout, "%5s", "|");
  for (i = 0; i < n; i++){
    fprintf(stdout, "%s", "-----");
  }
  fprintf(stdout, "\n");

  //print the graph
  for (row = 0; row < n; row++){
    //print header column with row number
    fprintf(stdout, "%3d%2s", row, "|");

    for (col = 0; col < n; col++){
      fprintf(stdout, "%5d", A[row][col]);
    }
    fprintf(stdout, "\n");
  }
}
