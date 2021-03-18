#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Function declaration
void ProcessCommandLine(int argc, char *argv[], int *num, char *ofile[]);

int main (int argc, char *argv[]){
  int i = 0;
  int num_integers = 100;
  int *list = NULL;
  char *output_file = "default-list-file.dat";

  //parse command line arguments
  ProcessCommandLine(argc, argv, &num_integers, &output_file);

  //generate array of integers
  list = (int *) malloc(sizeof(int) * (num_integers + 1));
  list[0] = num_integers;
  for (i = 1; i <= num_integers; i++){
    list[i] = i;
  }

  //open output file for writing
  FILE *ofptr = fopen(output_file, "wb");
  if (ofptr == NULL){
    fprintf(stderr, "Could not open %s\n", output_file);
    exit(-1);
  }

  //generate output file with list of integers
  fwrite(list, sizeof(int), num_integers + 1, ofptr);

  free(list);

  fclose(ofptr);

  return 0;
}

void ProcessCommandLine(int argc, char *argv[], int *num, char *ofile[]){
    int c;
    while ((c = getopt(argc, argv, "n:o:")) != -1){
        switch(c){
          case 'n':
            *num = atoi(optarg);
            break;
          case 'o':
            *ofile = strdup(optarg);
            break;
      }
    }
}
