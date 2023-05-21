#include <stdio.h>
#include <stdlib.h>

int main() {
    int size = 5;

    double* values = (double*)malloc(size * sizeof(double));
    if (values == NULL) {
        printf("Failed to allocate memory for the array.\n");
        return 1;
    }
    printf("%ld\n", sizeof(&values));
    printf("%ld\n", sizeof(values[0]));
    printf("Length=%ld\n", (sizeof(values)/sizeof(values[0])));

    return 0;
}
