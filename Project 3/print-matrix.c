/* Sarah Anderson
  ECE 4730: Parallel Programming
  Project 3: Cannons Algorithm
  Due: December 1, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void print_matrix(int r, int c, double ** A) {
    int i,j;
    printf("Array is a %d x %d matrix\n\n",r,c);
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
                    printf("%10.3f ",A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void read_matrix(char *file_name, int *r, int *c, double ***A) {
    FILE *input;
    double **AStorage;
    int i, j;

    input = fopen(file_name,"r");
    
    if(input == NULL) {
        printf("encountered error opening file\n");
        exit(0);
    }

    if(fread(r,sizeof(int),1,input) != 1) {
        printf("error reading matrix row size\n");
        exit(0);
    }
    
    if(fread(c,sizeof(int),1,input) != 1) {
        printf("error reading matrix column size\n");
        exit(0);
    }

    printf("Reading... Rows = %d; Cols = %d;\n",*r,*c);

    AStorage = (double **) malloc(*r * sizeof(double *));
    if(AStorage == NULL) {
        printf("matrix malloc failed \n");
        exit(0);
    }
    
    for(i = 0; i <*r; i++) {
        AStorage[i] = (double *)malloc(*c * sizeof(double));
        if(AStorage[i] == NULL) {
            printf("failed to allocate matrix columns\n");
            exit(0);
        }
    }
    
    for(i = 0; i < *r; i++) {
        for(j = 0; j < *c; j++) {
            if(fread(&AStorage[i][j],sizeof(double),1,input) != 1) {
                printf("error reading matrix value \n");
                exit(0);
            }
        }
    }

    *A = AStorage;
    fclose(input);
    return;
}

int main(int argc, char * argv[]) {

    char * inputname;
    double ** storage;
    int r,c,i;    
    if(argc != 2) {
        printf("Usage: ./exec -i input-file-name.dat\n");
        exit(0);
    }
    else {
        inputname = argv[1];
        printf("Reading from file %s...\n",inputname);
    }

    read_matrix(inputname,&r,&c,&storage);
    print_matrix(r,c,storage);

    for(i = 0; i < r; i++) {
			free(storage[i]);
    }
    free(storage);
    return 0;
}
