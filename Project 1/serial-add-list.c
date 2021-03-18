#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

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

int main (int argc, char *argv[]){
  int i, num_elements, array_sum = 0;
  char *input_file = "default-list-file.dat";
  int *file_as_array;

  time_t start = time(NULL);

  //parse command line arguments
  ProcessCommandLine(argc, argv, &input_file);

  //open input file for reading
  FILE *ifptr = fopen(input_file, "rb");
  if (ifptr == NULL){
    fprintf(stderr, "Could not open %s\n", input_file);
    exit(-1);
  }

  //read first line of array to get the number of elements
  fread(&num_elements, sizeof(int), 1, ifptr);

  //read list into array
  file_as_array = (int *) malloc(sizeof(int) * num_elements);
  fread(file_as_array, sizeof(int), num_elements, ifptr);

  //sum elements in array
  for (i = 0; i < num_elements; i++){
    array_sum += file_as_array[i];
  }
  fprintf(stdout, "Sum of array: %d\n", array_sum);

  //free file array
  free(file_as_array);

  time_t finish = time(NULL);

  fprintf(stdout, "total time: %ld\n", finish - start);

  return 0;
}
