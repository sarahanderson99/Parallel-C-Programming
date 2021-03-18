#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Function declariation
void ProcessCommandLine(int argc, char *argv[], char *infile[]);

int main (int argc, char *argv[]){
  int i, file_size, num_elements;
  char *input_file = "default-list-file.dat";
  int *file_as_array;

  //parse command line arguments
  ProcessCommandLine(argc, argv, &input_file);

  //open input file for reading
  FILE *ifptr = fopen(input_file, "rb");
  if (ifptr == NULL){
    fprintf(stderr, "Could not open %s\n", input_file);
    exit(-1);
  }

  //read file into array
  fseek(ifptr, 0, SEEK_END);
  file_size = ftell(ifptr);
  num_elements = file_size / sizeof(int);
  rewind(ifptr);
  file_as_array = (int *) malloc(sizeof(int) * file_size);
  fread(file_as_array, sizeof(int), file_size, ifptr);

  //print input file to console
  for (i = 0; i < num_elements; i++){
    fprintf(stdout, "%d\n", file_as_array[i]);
  }

  free(file_as_array);

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
