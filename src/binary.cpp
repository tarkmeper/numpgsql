extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type_d.h"
	#include "utils/array.h"
	#include "fmgr.h"
	#include "math.h"
}
#include "array.h"

#include <functional>
#include <tuple>

// Template to apply operation to all elements in the vector. Takes the appropriate function and
// applies it to every element in the array.
template< template <class T> class OP, class T >
static Datum _apply(
  ArrayType* res_a,
  T* a,
  T* b,
  size_t n
) {
	T* res = (T*) ARR_DATA_PTR(res_a);
        OP<T> op;
	for (unsigned int i = 0; i < n; ++i ) {
		res[i] = op(a[i], b[i]);
	}
	PG_RETURN_POINTER(res_a);
}

template<template <class T> class OP> Datum _op(PG_FUNCTION_ARGS) {
  ArrayType* arr_a = PG_GETARG_ARRAYTYPE_P(0);
  ArrayType* arr_b = PG_GETARG_ARRAYTYPE_P(1);

  //Check types
  Oid type_a = ARR_ELEMTYPE(arr_a);
  Oid type_b = ARR_ELEMTYPE(arr_b);
  if (type_a != type_b) {
     elog(ERROR, "Mismatched types not supported");
     PG_RETURN_NULL();
  }

  //Check array sizes
  const size_t n = arr_cnt(arr_a);
  const size_t nb = arr_cnt(arr_b);

  if (n == 0 ) { PG_RETURN_POINTER(arr_b); }
  else if (nb == 0 ) { PG_RETURN_POINTER(arr_a); }
  else if (n != nb) {
    elog(ERROR, "Array length mismatch");
    PG_RETURN_NULL();
  }

  void* a = ARR_DATA_PTR(arr_a);
  void* b = ARR_DATA_PTR(arr_b);
  ArrayType* res = PG_GETARG_ARRAYTYPE_P_COPY(0);

  switch(type_a) {
    case FLOAT4OID: return _apply< OP >(res, (float4*)a, (float4*)b, n );
    case FLOAT8OID: return _apply< OP >(res, (float8*)a, (float8*)b, n );
    case INT2OID: return _apply< OP >(res, (int16*)a, (int16*)b, n );
    case INT4OID: return _apply< OP >(res, (int32*)a, (int32*)b, n );
    case INT8OID: return _apply< OP >(res, (int64*)a, (int64*)b, n );
    default:
      elog(ERROR, "Unsupported array type");
      PG_RETURN_NULL();
  }
}

//Helper function to encapsulate the boiler plate code needed on each function definition.  This is pretty ugly
//not sure if there is a better way to specificy this using templates that would also allow
//for potentially inlining.
#define BINARY_FNC(NAME, OP)		\
	extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); }	\
        Datum NAME(PG_FUNCTION_ARGS) { return _op<OP>(fcinfo); }


BINARY_FNC(plus, std::plus);
BINARY_FNC(minus, std::minus);
BINARY_FNC(multiply, std::multiplies);
BINARY_FNC(divide, std::divides);
//BINARY_FNC(modulus, std::modulus);
