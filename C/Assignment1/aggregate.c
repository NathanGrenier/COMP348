#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aggregate.h"
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

static double _count(double* arr, int size) {
    return (double)size;
}

static double _min(double* arr, int size) {
    double min = arr[0];
    for (int i=1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

static double _max(double* arr, int size) {
    double max = arr[0];
    for (int i=1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

static double _sum(double* arr, int size) {
    double sum;
    for (int i=0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

static double _avg(double* arr, int size) {
    return _sum(arr, size) / size;
}

static double _pavg(double* arr, int size) {
    return (_min(arr, size) + _max(arr, size)) / 2;
}


// The following declares an array of pointer
static double (*farray[6])(double*, int) = {&_count, &_min, &_max, &_sum, &_avg, &_pavg};
// The following declares an array of strings
static const char* funcnames[] = {"COUNT", "MIN", "MAX", "SUM", "AVG", "PAVG"};

double aggregate(const char* func, double* arr, int size) {
    if (arr == NULL || size < 0) {
        fprintf (stderr, "Fatal Error: inputed array is Null or size is negative, in file: %s, on line: %d.\n", __FILE__, __LINE__);
        exit(1);
    }

    // Convert passed function name to uppercase
    char funcname[LENGTH(func)];
    for (int i=0; i < LENGTH(funcname); i++) {
        funcname[i] = toupper(func[i]);
    }

    for (int i=0; i < LENGTH(funcnames); i++) {
        if (strcmp(funcname, funcnames[i]) == 0) {
            return (*farray[i])(arr, size);
        }
    }
    
    // No aggregate function was with that name, diplay an error
    fprintf (stderr, "Fatal Error: no function with the name %s was found, in file: %s, on line: %d.\n", func, __FILE__, __LINE__);
    exit(1);
}