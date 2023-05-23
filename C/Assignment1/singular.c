#include <stdio.h>
#include "singular.h"
#define filterTypeCount 6

const int filter_types_count = filterTypeCount;
char *filter_types[filterTypeCount] = {"EQ", "NEQ", "GEQ", "LEQ", "LESS", "GREATER"};

void print(double a[], size_t size) {
    if (size == 0) {return;}
    for (int i=0; i < size; i++) {
        printf("%*.*f ", output_size, output_prec, a[i]);
    }
    printf("\n");
}

void shift(double a[], size_t size, double by) {
    for (int i=0; i < size; i++) {
        a[i] += by;
    }
    print(a, size);
}

size_t filter(double a[], size_t count, enum filter_type t, double threshold) {
    int matches = 0;
    for (int i=0; i < count; i++) {
        switch(t) {
            case EQ:
                if (a[i] == threshold) {
                    printf("%*.*f ", output_size, output_prec, a[i]);
                    matches++;
                }
                break; 
            case NEQ:
                if (a[i] != threshold) {
                    printf("%*.*f ", output_size, output_prec, a[i]);
                    matches++;
                }
                break;     
            case GEQ:
                if (a[i] >= threshold) {
                    printf("%*.*f ", output_size, output_prec, a[i]);
                    matches++;
                }
                break; 
            case LEQ:
                if (a[i] <= threshold) {
                    printf("%*.*f ", output_size, output_prec, a[i]);
                    matches++;
                }
                break; 
            case LESS:
                if (a[i] < threshold) {
                    printf("%*.*f ", output_size, output_prec, a[i]);
                    matches++;
                }
                break; 
            case GREATER:
                if (a[i] > threshold) {
                    printf("%*.*f ", output_size, output_prec, a[i]);
                    matches++;
                }
                break; 
            default:
                break;
        }
    }
    printf("\n");
    return matches;
}
