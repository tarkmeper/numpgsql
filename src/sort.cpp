extern "C"  {
	#include "postgres.h"
	#include "utils/array.h"
	#include "fmgr.h"
        #include <utils/lsyscache.h>

	PG_FUNCTION_INFO_V1(sort); Datum sort(PG_FUNCTION_ARGS);
	PG_FUNCTION_INFO_V1(reverse); Datum reverse(PG_FUNCTION_ARGS);
}
#include "array.h"

#include <algorithm>
#include <tuple>

template<class T> void do_sort(void* v, const size_t n) {
    T* f = reinterpret_cast<T*>(v);
    std::sort(f, f + n );
}

/***** operator template ****/
Datum sort(PG_FUNCTION_ARGS) {
	ArrayType* arr = PG_GETARG_ARRAYTYPE_P_COPY(0);

        const size_t n = arr_cnt(arr);
        if (n == 0 ) { PG_RETURN_POINTER(arr); }

        Oid type = ARR_ELEMTYPE(arr);
        void * v = ARR_DATA_PTR(arr);

        switch(type) {
            case FLOAT4OID: do_sort< float > (v, n); break;
            case FLOAT8OID: do_sort< double > (v, n); break;
            case INT2OID: do_sort< int16 > (v, n); break;
            case INT4OID: do_sort< int32 > (v, n); break;
            case INT8OID: do_sort< int64 > (v, n); break;
            default:
                elog(ERROR, "Unsupported array type");
                PG_RETURN_NULL();
        }
	PG_RETURN_POINTER(arr);
}


//There may be a slightly better version that can do this swap in place.
Datum reverse(PG_FUNCTION_ARGS) {
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0);

  Oid        type  = ARR_ELEMTYPE(arr);
  int16      elmlen;
  bool       elmbyval;
  char       elmalign;
  Datum*     elemsp;
  bool*      nullsp;
  int        nelemsp;

  if (ARR_NDIM(arr) == 0 ) {
     PG_RETURN_POINTER(construct_empty_array(type));
  } else if (ARR_NDIM(arr) != 1 ) {
     elog(ERROR, "Currently reverse only support 1d arrays");
     PG_RETURN_NULL();
  }

  const size_t n = ARR_DIMS(arr)[0];
  get_typlenbyvalalign(type, &elmlen, &elmbyval, &elmalign);
  deconstruct_array(arr, type, elmlen, elmbyval, elmalign, &elemsp, &nullsp, &nelemsp);

  Datum* rev = (Datum*)palloc(n * sizeof(Datum));
  std::copy(elemsp, elemsp + n, rev);
  std::reverse(rev, rev + n);

  ArrayType* res = construct_array(rev, n, type, elmlen, elmbyval, elmalign);
  PG_RETURN_POINTER(res);
}
