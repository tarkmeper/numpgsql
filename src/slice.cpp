extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type.h"
	#include "utils/array.h"
        #include <utils/lsyscache.h>
	#include "fmgr.h"
	#include "math.h"
}
#include "array.h"
#include <functional>
#include <tuple>

extern "C" { PG_FUNCTION_INFO_V1(slice_idx); Datum slice_idx(PG_FUNCTION_ARGS); }

Datum slice_idx(PG_FUNCTION_ARGS) {
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0);
  ArrayType* idx_arr = PG_GETARG_ARRAYTYPE_P(1);

  Oid        type  = ARR_ELEMTYPE(arr);
  int16      elmlen;
  bool       elmbyval;
  char       elmalign;
  Datum*     elemsp;
  bool*      nullsp;
  int        nelemsp;

  int* idx = (int*)ARR_DATA_PTR(idx_arr);
  if (ARR_NDIM(idx_arr) == 0 ) {
     PG_RETURN_POINTER(construct_empty_array(type));
  }
  const size_t n = ARR_DIMS(idx_arr)[0];
  get_typlenbyvalalign(type, &elmlen, &elmbyval, &elmalign);
  deconstruct_array(arr, type, elmlen, elmbyval, elmalign, &elemsp, &nullsp, &nelemsp);

  Datum* slice = (Datum*)palloc(n * sizeof(Datum));
  for (size_t i = 0; i < n; ++i) {
     int x = idx[i];
     if (x < 0) { x = nelemsp + x + 1; } 
     if (x <= 0 || x > nelemsp) {
       elog(ERROR, "Index entry %d is out of array bounds", idx[i] );
       PG_RETURN_NULL();
     }
     slice[i] = elemsp[x - 1]; //SQL and C covnention is different
  }
  ArrayType* res = construct_array(slice, n, type, elmlen, elmbyval, elmalign);
  PG_RETURN_POINTER(res);
}


extern "C" { PG_FUNCTION_INFO_V1(slice_bit); Datum slice_bit(PG_FUNCTION_ARGS); }

Datum slice_bit(PG_FUNCTION_ARGS) {
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0);
  ArrayType* idx_arr = PG_GETARG_ARRAYTYPE_P(1);

  Oid        type  = ARR_ELEMTYPE(arr);
  int16      elmlen;
  bool       elmbyval;
  char       elmalign;
  Datum*     elemsp;
  bool*      nullsp;
  int        nelemsp;

  bool* idx = (bool*)ARR_DATA_PTR(idx_arr);
  if (ARR_NDIM(idx_arr) == 0 ) {
     elog(ERROR, "Can't use empty array as index");
     PG_RETURN_NULL();
  }

  const size_t n = ARR_DIMS(idx_arr)[0];
  get_typlenbyvalalign(type, &elmlen, &elmbyval, &elmalign);
  deconstruct_array(arr, type, elmlen, elmbyval, elmalign, &elemsp, &nullsp, &nelemsp);

  if (n != (size_t)nelemsp) {
    elog(ERROR, "Masking array must be same length as input array");
    PG_RETURN_NULL();
  }
  size_t cnt  = 0;
  for (size_t i = 0; i < n; ++i ) { if (idx[i]) { cnt++; } };

  if (cnt == 0) {
     PG_RETURN_POINTER(construct_empty_array(type));
  }

  Datum* slice = (Datum*)palloc(cnt * sizeof(Datum));
  size_t offset = 0;
  for (size_t i = 0; i < n; ++i) {
     if (idx[i]) {
        slice[offset++] = elemsp[i];
     }
  }
  ArrayType* res = construct_array(slice, cnt, type, elmlen, elmbyval, elmalign);
  PG_RETURN_POINTER(res);
}
