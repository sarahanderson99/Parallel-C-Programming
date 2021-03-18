/*
Sarah Anderson
ECE 4730: Parallel Systems
Floyds Algor
Due: Nove 7th, 2020
*/

#define ROOT 0

int IsEdge(int vertex_a, int vertex_b);

void read_graph(char *file_name, int *n, int ***A);

void write_graph_serial(char *file_name, int n, int **A);

void write_graph_parallel(char *file_name, int rank, int n, void **A, MPI_Comm grid_comm);

void print_graph(int n, int **A);
