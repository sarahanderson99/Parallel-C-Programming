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

void write_matrix(char *file_name, int r, int c, double ***A) {
    FILE *output;
    int i,j;
    double temp;
    output = fopen(file_name,"w");
    if(output == NULL) {
        printf("encountered error opening file \n");
        exit(0);
    }

    if(fwrite(&r,sizeof(int),1,output) != 1) {
        printf("error writing matrix rows\n");
        exit(0);
    }
    if(fwrite(&c,sizeof(int),1,output) != 1) {
        printf("error writing matrix columns\n");
        exit(0);
    }

    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            temp = (*A)[i][j];
            if(fwrite(&temp,sizeof(double),1,output) != 1) {
                printf("error reading matrix value\n");
                exit(0);
            }
        }
    }
    fclose(output);
    
    for(i =0; i < r; i++) {
        free((*A)[i]);
    }
    free(*A);
    *A = NULL;
    return;
}

int main(int argc, char * argv[]) {

    int opt, i, j;
    int rows, cols, low, up;
    double val;
    char *outputname;
    double **Mstorage;

    if(argc != 11) {
        printf("Usage: ./exec -r <rows> -c <cols> -l <low> -u <up> -o <fname>\n");
        exit(0);
    }

    while((opt = getopt(argc,argv,"r:c:l:u:o:")) != -1) {
        switch(opt) {
            case 'r':
                rows = atoi(optarg);
                break;
            case 'c':
                cols = atoi(optarg);
                break;
            case 'l':
                low = atoi(optarg);
                break;
            case 'u':
                up = atoi(optarg);
                break;
            case 'o':
                outputname = optarg;
                break;
            default:
                printf("Usage: ./exec -r <rows> -c <cols> -l <low> -u <up> -o <fname>\n");
                exit(0);
        }
    }

    Mstorage = (double **) malloc(rows * sizeof(double *));
    if(Mstorage == NULL) {
	printf("failed to allocate matrix\n");
	exit(0);
    }
    for(i = 0; i < rows; i++) {
	    Mstorage[i] = (double *) malloc(cols * sizeof(double));
	    if(Mstorage[i] == NULL) {
		    printf("failed to allocate matrix\n");
		    exit(0);
	    }
    }

    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
						val = ((double)low + ((double)rand() / (up - low))) / 1000.0;
            Mstorage[i][j] = val;  
        }
    }
    write_matrix(outputname,rows,cols,&Mstorage);	
    return 0;
}
