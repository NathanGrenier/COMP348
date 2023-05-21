#pragma once
#include <stdlib.h>

enum filter_type { EQ = 0, NEQ = 1, GEQ = 2, LEQ = 3, LESS = 4, GREATER = 5 } t;

void print(double a[], size_t size);
void shift(double a[], size_t size, double by);
size_t filter(double a[], size_t count, enum filter_type t, double threshold);