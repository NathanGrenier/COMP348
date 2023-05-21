#include <stdio.h>
#include <stdlib.h>
#include "aggregate.h"

static double _min(double* arr, int size) {

}


double aggregate(const char* func, double* arr, int size) {
    if (arr == NULL || size < 0) {
        fprintf (stderr, "Internal error: inputed array is Null or size is negative, in file: %s, on line: %d.\n", __FILE__, __LINE__);
        exit(1);
    }
}