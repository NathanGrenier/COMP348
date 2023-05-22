#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aggregate.h"
#include "singular.h"
#define MAX_COL 256
#define MAX_CHAR 2048   // Safe estimate of max buffer size

int main(int argc, char *argv[])  {
    char input[MAX_CHAR];
    char delimiter[] = " \n";
    double **arr = (double **)malloc(1 * sizeof(double *));
    int *sizes = (int *)malloc(1 * sizeof(int));
    double *innerArr;

    int row = 0;
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strcmp(input, "\n") == 0) {continue;}
        innerArr = (double *)malloc(MAX_COL * sizeof(double));
        if (innerArr == NULL) {
            printf("Failed to allocate memory for the array.\n");
            return 1;
        }

        int col = 0;
        double scanned;
        char* token = strtok(input, delimiter);
        while (token != NULL) {
            if (col == 256) {
                arr[row] = innerArr;
                sizes[row] = col;  
                row++;
                sizes = (int *)realloc(sizes, (row + 1) * sizeof(int));
                arr = (double **)realloc(arr, (row + 1) * sizeof(double *));
                col = 0;
                innerArr = (double *)malloc(MAX_COL * sizeof(double));
                if (innerArr == NULL) {
                    printf("Failed to allocate memory for the array.\n");
                    return 1;
                }
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

    for (int i=0; i < row; i++) {
        //printf("%f ", aggregate("", arr[i], sizes[i]));
        print(arr[i], sizes[i]);
    }
    printf("\n");

    for (int i=0; i < row; i++) {
        free(arr[row]);
    }
    free(arr);
}