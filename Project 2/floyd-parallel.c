/*
Sarah Anderson
ECE 4730: Parallel Systems
Floyds Algor
Due: Nove 7th, 2020
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <unistd.h>
#include "graph.h"
#include "MyMPI.h"

//A parallel implementation of Floyd's algorithm using a 2D checkerboard matrix
void floyd_parallel (int rank, int size, int ***graph, int nodes, int grid_coords[], int grid_size[], MPI_Comm row_comm, MPI_Comm col_comm){

   int i, j, k;
   int owner, grid_id;
   int offset;
   int local_rows = BLOCK_SIZE(grid_coords[0],grid_size[0],nodes);
   int local_cols = BLOCK_SIZE(grid_coords[1],grid_size[1],nodes);
   int* col_k = (int *) malloc(local_rows * sizeof(int));
   int* row_k = (int *) malloc(local_cols * sizeof(int));

   for (k = 0; k < nodes; k++) {
     //get the grid id of the current process in the col communicator
     MPI_Comm_rank(col_comm, &grid_id);

     //get the owner of the kth row
     owner = BLOCK_OWNER(k, grid_size[0], nodes);

     //process 'rank' owns the kth row
     if (grid_id == owner){
       //print individual graphs
       offset = k - BLOCK_LOW(grid_id, grid_size[0], nodes);
       for (i = 0; i < local_cols; i++){
         row_k[i] = (*graph)[offset][i];
       }
     }
     MPI_Bcast(row_k, local_cols, MPI_INT, owner, col_comm);

     //get the grid id of the current process in the row communicator
     MPI_Comm_rank(row_comm, &grid_id);

     //get the owner of the kth column
     owner = BLOCK_OWNER(k, grid_size[1], nodes);

     //process with grid_id owns the kth column
     if (grid_id == owner){
       //print individual graphs
       offset = k - BLOCK_LOW(grid_id, grid_size[1], nodes);
       for (i = 0; i < local_rows; i++){
         col_k[i] = (*graph)[i][offset];
       }
     }
     MPI_Bcast(col_k, local_rows, MPI_INT, owner, row_comm);

		for (i = 0; i < local_rows; i++){
			for (j = 0; j < local_cols; j++){
			 if (
					(IsEdge(row_k[j], col_k[i]) && (*graph)[i][j] == -1)
																 ||
					(IsEdge(row_k[j], col_k[i]) && (row_k[j] + col_k[i]) < (*graph)[i][j])
				  )
					(*graph)[i][j] = row_k[j] + col_k[i];
			}
		}
  }
   free(row_k);
   free(col_k);
}


int main (int argc, char *argv[]){
 
  int **graph, *indiv_graph;
  int rank, size, nodes;
  double prog_exec_time, floyd_exec_time;

  //cartesian topology variables
  MPI_Comm grid_comm, row_comm, col_comm;
  int grid_id;
  int grid_size[2] = {0, 0};
  int periodic[2] = {0, 0};
  int grid_coords[2] = {0, 0};

  //initialize
  MPI_Init(&argc, &argv);

  //set the start time of the program
  MPI_Barrier(MPI_COMM_WORLD);
  prog_exec_time = -MPI_Wtime();

  //get the number of MPI tasks
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  //get the current MPI task number
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //verify command line arguments
  if (argc < 3){
    if (rank == ROOT){
  	   fprintf(stdout, "Program execution requires input file and output file arguments.\n");
    }
  	MPI_Finalize();
  	exit(-1);
  }

  //create the grid communicator
  MPI_Dims_create(size, 2, grid_size);
  MPI_Cart_create(MPI_COMM_WORLD, 2, grid_size, periodic, 0, &grid_comm);

  //get the coordinates for a given id
  MPI_Comm_rank(grid_comm, &grid_id);
  MPI_Cart_coords(grid_comm, grid_id, 2, grid_coords);

  //create the row and column communicators
  MPI_Comm_split(grid_comm, grid_coords[0], grid_coords[1], &row_comm);
  MPI_Comm_split(grid_comm, grid_coords[1], grid_coords[0], &col_comm);

  //read in the graph
  if (rank == ROOT){
    fprintf(stdout, "reading graph from file %s\n", argv[1]);
  }
  read_checkerboard_matrix (argv[1], (void ***) &graph, (void **) &indiv_graph, MPI_INT, &nodes, grid_comm);

  //set the start time of the floyd parallel algorithm
  MPI_Barrier(MPI_COMM_WORLD);
  floyd_exec_time = -MPI_Wtime();

  //run floyds algorithm
  floyd_parallel(rank, size, &graph, nodes, grid_coords, grid_size, row_comm, col_comm);

  //set the end time of the floyd parallel algorithm
  MPI_Barrier(MPI_COMM_WORLD);
  floyd_exec_time += MPI_Wtime();

  //output shortest path graph to output file
  MPI_Barrier(MPI_COMM_WORLD);
  write_graph_parallel(argv[2], rank, nodes, (void **) graph, grid_comm);

  //free communicators
  MPI_Comm_free(&row_comm);
  MPI_Comm_free(&col_comm);
  MPI_Comm_free(&grid_comm);

  //set the end time of the program
  MPI_Barrier(MPI_COMM_WORLD);
  prog_exec_time += MPI_Wtime();

  //output timing results
  if (rank == ROOT){
    fprintf(stdout, "floyd-parallel execution time:\n");
    fprintf(stdout, "\tn = %5d nodes\n", nodes);
    fprintf(stdout, "\tp = %5d cpus\n", size);
    fprintf(stdout, "\tptime = %6.5f (sec)\n", prog_exec_time);
    fprintf(stdout, "\tftime = %6.5f (sec)\n", floyd_exec_time);
  }

  //exit
  MPI_Finalize();
  return 0;
}
