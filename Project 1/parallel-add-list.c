#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>
#include "MyMPI.h"
#include "parallel-add-list.h"

//Function declaration section
//void ProcessCommandLine(int argc, char *argv[], char *infile[]);
//int IsPowerOfTwo(int size);
//void print_array (void *a, int n, int rank);
//void individual_sum(void *a, int rank, double *my_value, int n);
//void global_sum(double *result, int rank, int size, double my_value);
//int root;

int main (int argc, char *argv[]){
  int rank, size, vector_length;
  char *input_file = "default-list-file.dat";
  void *out_vector;
  double my_value, result = 0;
  double starttime = 0, endtime = 0;
 
 
  //parse command line arguments
  ProcessCommandLine(argc, argv, &input_file);

  //initializes the command line arguments
  MPI_Init(&argc, &argv);

  starttime = MPI_Wtime();

  //gets the num of MPI tasks
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  //define the root process
  root = size - 1;

  //get the current MPI task id (rank)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //number of tasks must be a power of 2
  if (IsPowerOfTwo(size)){
    if (rank == root){
      fprintf(stderr, "Number of tasks must be a power of 2\n");
    }

    MPI_Finalize();
    exit(-1);
  }

  //for debugging; print MPI task with process ID
  fprintf(stdout, "** MPI task %d has started with PID: %d **\n", rank, getpid());

  //use block decomposition to evenly distribute elements across processes
  read_block_vector(input_file, &out_vector, MPI_INT, &vector_length, MPI_COMM_WORLD);

  //for debugging; print out subvector of each process
  print_array(out_vector, BLOCK_SIZE(rank, size, vector_length), rank);

  //get individual sums
  individual_sum(out_vector, rank, &my_value, BLOCK_SIZE(rank, size, vector_length));

  //pass individual sums into global sum
  global_sum(&result, rank, size, my_value);

  MPI_Barrier(MPI_COMM_WORLD);

  endtime = MPI_Wtime();

  printf("Total time: %f\n\n", endtime - starttime);
  
  //exit
  MPI_Finalize();

  return 0;
}

void ProcessCommandLine(int argc, char *argv[], char *infile[]){
    int c;
    
    while ((c = getopt(argc, argv, "i:")) != -1){
      switch(c){
          case 'i':
            *infile = strdup(optarg);
            break;
      }
    }
}

//checks if the command line arg is a power of 2
int IsPowerOfTwo(int size)
{
  if ((size & (size - 1)) == 0) return 0;
  else return 1;
}

//prints the values of the array
void print_array (void *a, int n, int rank){
   int i;
    
   fprintf(stdout, "Process %d subvector:\n", rank);
   for (i = 0; i < n; i++) {
      printf ("%6d ", ((int *)a)[i]);
   }
   fprintf(stdout, "\n");
}

void individual_sum(void *a, int rank, double *my_value, int n){
    int i;
    *my_value = 0;

    //calculate sum
    for (i = 0; i < n; i++){
      *my_value += ((int *)a)[i];
    }
}

//calcu;ates global sum by suming up small vectors
void global_sum(double *result, int rank, int size, double my_value){
  int i;
  MPI_Status status;

  //root process computes the global sum
  if (rank == root)
  {
      //get root's individual sum
      *result += my_value;

      //sum up all individual sums from all other processes
      for (i = 0; i < root; i++){
          MPI_Recv(&my_value, 1, MPI_DOUBLE, i, DATA_MSG, MPI_COMM_WORLD, &status);
          *result += my_value;
      }
      fprintf(stdout, "Process %d (ROOT) sent global sum: %f\n", rank, *result);

      //broadcast global sum to all other processes
      for (i = 0; i < root; i++){
        MPI_Send(result, 1, MPI_DOUBLE, i, DATA_MSG, MPI_COMM_WORLD);
      }
  }
  //other processes send individual sums to root processes to be summed into global sum
  else
  {
      //send individual sum
      MPI_Send(&my_value, 1, MPI_DOUBLE, root, DATA_MSG, MPI_COMM_WORLD);

      //wait for global sum
      MPI_Recv(result, 1, MPI_DOUBLE, root, DATA_MSG, MPI_COMM_WORLD, &status);

      //print global sum
      fprintf(stdout, "Process %d received global sum: %f\n", rank, *result);
  }
}
