void ProcessCommandLine(int argc, char *argv[], char *infile[]);
int IsPowerOfTwo(int size);
void print_array (void *a, int n, int rank);
void individual_sum(void *a, int rank, double *my_value, int n);
void global_sum(double *result, int rank, int size, double my_value);
int root;

