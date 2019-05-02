extern "C"  {
        #include "postgres.h"
        #include "catalog/pg_type.h"
        #include "fmgr.h"
        #include "math.h"
        #include <utils/lsyscache.h>
}
#include "random.h"
#include "../array.h"

#include <algorithm>


extern "C" { PG_FUNCTION_INFO_V1(permute); Datum permute(PG_FUNCTION_ARGS); }
Datum permute(PG_FUNCTION_ARGS) {
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0);
  Oid        type  = ARR_ELEMTYPE(arr);
  int16      elmlen;
  bool       elmbyval;
  char       elmalign;
  Datum*     elemsp;
  bool*      nullsp;
  int        nelemsp;

  if (ARR_NDIM(arr) == 0 ) {
     PG_RETURN_POINTER(arr);
  } else if (ARR_NDIM(arr) != 1 ) {
     elog(ERROR, "PERMUTE: Currently only supports 1d arrays");
     PG_RETURN_NULL();
  }

  const size_t n = ARR_DIMS(arr)[0];
  get_typlenbyvalalign(type, &elmlen, &elmbyval, &elmalign);
  deconstruct_array(arr, type, elmlen, elmbyval, elmalign, &elemsp, &nullsp, &nelemsp);

  Datum* rev = (Datum*)palloc(n * sizeof(Datum));
  std::copy(elemsp, elemsp + n, rev);
  std::shuffle(rev, rev+n, rgen);

  ArrayType* res = construct_array(rev, n, type, elmlen, elmbyval, elmalign);
  PG_RETURN_POINTER(res);
}

// Choice.
extern "C" { PG_FUNCTION_INFO_V1(choice); Datum choice(PG_FUNCTION_ARGS); }
Datum choice(PG_FUNCTION_ARGS) {
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0);
  ArrayType* idx = PG_GETARG_ARRAYTYPE_P(1);
  bool replace = get_param<bool>(fcinfo, 2);
  Oid        type  = ARR_ELEMTYPE(arr);
  int16      elmlen;
  bool       elmbyval;
  char       elmalign;
  Datum*     elemsp;
  bool*      nullsp;
  int        nelemsp;

  //cehck for errors etc.
  if (ARR_NDIM(arr) > 1) {
     elog(ERROR, "CHOICE: Currently only supports 1d arrays");
     PG_RETURN_NULL();
  } else if (ARR_NDIM(arr) == 0 ) {
     elog(ERROR, "CHOICE: Input array must ahve at least 1 element");
     PG_RETURN_NULL();
  } else if (ARR_NDIM(idx) != 1 ) {
     elog(ERROR, "CHOICE: Size array must be 1d array");
     PG_RETURN_NULL();
  } else if (ARR_DIMS(idx)[0] >= MAXDIM) {
     elog(ERROR, "CHOICE: Multi-dimension array must have less than %d dimensions has %d", MAXDIM, ARR_DIMS(idx)[0]);
     PG_RETURN_NULL();
  }

  const size_t input_n = ARR_DIMS(arr)[0];
  get_typlenbyvalalign(type, &elmlen, &elmbyval, &elmalign);
  deconstruct_array(arr, type, elmlen, elmbyval, elmalign, &elemsp, &nullsp, &nelemsp);

  ArrDims rd = extract_shape(idx);

  Datum* r = (Datum*)palloc(rd.n * sizeof(Datum));

  if (replace) {
     std::uniform_int_distribution<> dist(0, input_n-1);
     for (size_t i = 0; i < rd.n; ++i ) {
        r[i] = elemsp[dist(rgen)];
     }
  } else {
      if ( rd.n > input_n) { 
        elog(ERROR, "Without replacement target vector cannot be better then input");
        PG_RETURN_NULL();
      }
      std::vector<Datum> tmp(elemsp, elemsp + nelemsp);
      std::shuffle(tmp.begin(), tmp.end(), rgen);
      std::copy(tmp.begin(), tmp.begin() + rd.n, r);
  }

  ArrayType* res = construct_md_array(r,
                                      NULL,
                                      rd.ndims,
                                      rd.dims,
                                      rd.lbs,
                                      type,
                                      elmlen,
                                      elmbyval,
                                      elmalign);


  PG_RETURN_POINTER(res);
}

