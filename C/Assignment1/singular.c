#include <stdio.h>
#include "singular.h"

void print(double a[], size_t size) {
    if (size == 0) {return;}
    for (int i=0; i < size; i++) {
        printf("%f ", a[i]);
    }
    printf("\n");
}

void shift(double a[], size_t size, double by) {
    for (int i=0; i < size; i++) {
        a[i] += by;
    }
}

size_t filter(double a[], size_t count, enum filter_type t, double threshold) {
    for (int i=0; i < count; i++) {
        switch(t) {
            case EQ:
                if (a[i] == threshold) {printf("%f ", a[i]);}
                break; 
            case NEQ:
                if (a[i] != threshold) {printf("%f ", a[i]);}
                break;     
            case GEQ:
                if (a[i] >= threshold) {printf("%f ", a[i]);}
                break; 
            case LEQ:
                if (a[i] <= threshold) {printf("%f ", a[i]);}
                break; 
            case LESS:
                if (a[i] < threshold) {printf("%f ", a[i]);}
                break; 
            case GREATER:
                if (a[i] > threshold) {printf("%f ", a[i]);}
                break; 
            default:
                break;
        }
    }
}
