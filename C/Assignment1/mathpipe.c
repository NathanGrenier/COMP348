#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aggregate.h"
#define MAX_COL 256
#define MAX_CHAR 1000   // Safe estimate of max buffer size

int main(int argc, char *argv[])  {
    char input[MAX_CHAR];
    char delimiter[] = " \n";
    double **arr = (double **)malloc(1 * sizeof(double *));
    int *sizes = (int *)malloc(1 * sizeof(int));

    int row = 0;
    while (fgets(input, sizeof(input), stdin) != NULL) {
        double *innerArr = (double *)malloc(MAX_COL * sizeof(double));
        if (innerArr == NULL) {
            printf("Failed to allocate memory for the array.\n");
            return 1;
        }

        int col = 0;
        double scanned;
        char* token = strtok(input, delimiter);
        while (token != NULL) {
            if (col == 255) { // 256 is the max size an inner array can be. col starts at 0, therefore the max size is 255
                //TODO Handle the case where the array is over 256 numbers (make a new array)
                printf("Broke\n");
                break;
            }
            sscanf(token, "%lf", &scanned);
            innerArr[col] = scanned;
            col++;

            token = strtok(NULL, delimiter);
        }

        innerArr = (double *)realloc(innerArr, (col) * sizeof(double));
        if (innerArr == NULL) {
            printf("Failed to reallocate memory for the array.\n");
            return 1;
        }
        arr[row] = innerArr;
        sizes[row] = col;  
        row++;
        sizes = (int *)realloc(sizes, (row + 1) * sizeof(int));
        arr = (double **)realloc(arr, (row + 1) * sizeof(double *));
    }
    
    for (int i=0; i < row; i++) {
        for (int j=0; j < sizes[i]; j++) {
            printf("%.1f ", arr[i][j]);
        }
        printf("\n");
    }
}