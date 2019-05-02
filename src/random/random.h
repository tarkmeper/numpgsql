#ifndef RANDOM_H
#define RANDOM_H

#include <random>

extern std::mt19937 rgen;

struct ArrDims {
   int ndims;
   int* dims;
   int lbs[MAXDIM];
   size_t n;
};

extern ArrDims extract_shape(ArrayType* arr);

#endif
