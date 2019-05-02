extern "C"  {
        #include "postgres.h"
        #include "catalog/pg_type.h"
        #include "fmgr.h"
        #include "math.h"
}
#include <string>
#include <iostream>
#include <sstream>
#include "random.h"
#include "../array.h"

std::mt19937 rgen;

ArrDims extract_shape(ArrayType* arr) {
  static constexpr int LBS_VALS[] = { 1, 1, 1, 1, 1, 1 };
  ArrDims res;
  res.dims = (int*)(ARR_DATA_PTR(arr));
  res.ndims = ARR_DIMS(arr)[0];
  res.n = res.dims[0];
  memcpy(res.lbs, LBS_VALS, sizeof(int) * MAXDIM);
  for ( int i = 1; i < res.ndims; ++i) { res.n *= res.dims[i]; }
  return res;
}

extern "C" { PG_FUNCTION_INFO_V1(set_seed); Datum set_seed(PG_FUNCTION_ARGS); }
 Datum set_seed(PG_FUNCTION_ARGS) {
  const int s = PG_GETARG_INT32(0);
  rgen.seed(s);
  PG_RETURN_NULL();
}


extern "C" { PG_FUNCTION_INFO_V1(get_rgen_state); Datum get_rgen_state(PG_FUNCTION_ARGS); }
Datum get_rgen_state(PG_FUNCTION_ARGS) {
  std::stringstream ss;
  ss << rgen;

  const std::string val = ss.str();

  //Now move over
  text *new_t = (text *) palloc(val.size() + VARHDRSZ);
  SET_VARSIZE(new_t, val.size() + VARHDRSZ);
  memcpy(VARDATA(new_t), val.c_str(), val.size());

  PG_RETURN_TEXT_P(new_t);
}


extern "C" { PG_FUNCTION_INFO_V1(set_rgen_state); Datum set_rgen_state(PG_FUNCTION_ARGS); }
Datum set_rgen_state(PG_FUNCTION_ARGS) {
  text  *arg1 = PG_GETARG_TEXT_PP(0);  std::string val(VARDATA(arg1));
  std::istringstream ss(val);
  ss >> rgen;

  PG_RETURN_NULL();
}


template<class D> static Datum _gen_rand(PG_FUNCTION_ARGS, D& dist, int dim_arr_idx = 0) {
  typedef typename D::result_type T;
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P_COPY(dim_arr_idx);

  if (ARR_NDIM(arr) == 0 ) {
     PG_RETURN_POINTER(construct_empty_array(oid_type<double>()));
  } else if (ARR_NDIM(arr) != 1 ) {
     elog(ERROR, "GEN_RAND: Size specification can only be 1-dimension array");
     PG_RETURN_NULL();
  } else if (ARR_DIMS(arr)[0] >= 4 ) {
     elog(ERROR, "GEN_RAND: Maximum of 4 dimensions in array generation.");
     PG_RETURN_NULL();
  }

  //compute number of dimensions
  ArrDims rd = extract_shape(arr);
  Datum* r = (Datum*)palloc(rd.n * sizeof(Datum));
  for (size_t i = 0; i < rd.n; ++i ) {
     r[i] = _ret<T>(dist(rgen));
  }

  ArrayType* res = construct_md_array(r,
                                      NULL,
                                      rd.ndims,
                                      rd.dims,
                                      rd.lbs,
                                      oid_type<T>(),
                                      sizeof(T),
                                      true,
                                      oid_align<T>());
  PG_RETURN_POINTER(res);
}

extern "C" { PG_FUNCTION_INFO_V1(gen_rand); Datum gen_rand(PG_FUNCTION_ARGS); }
Datum gen_rand(PG_FUNCTION_ARGS) {
  std::uniform_real_distribution<double> dist{0.0, 1.0};
  return _gen_rand(fcinfo, dist);
}


extern "C" { PG_FUNCTION_INFO_V1(gen_randn); Datum gen_randn(PG_FUNCTION_ARGS); }
Datum gen_randn(PG_FUNCTION_ARGS) {
  std::normal_distribution<double> dist{0.0, 1.0};
  return _gen_rand(fcinfo, dist);
}


extern "C" { PG_FUNCTION_INFO_V1(gen_randint); Datum gen_randint(PG_FUNCTION_ARGS); }
Datum gen_randint(PG_FUNCTION_ARGS) {
  long low = get_param<long>(fcinfo, 0);
  long high = get_param<long>(fcinfo, 1);
  if (high <= low ){
     elog(ERROR, "GEN_RANDINT: Cannot have high bound lower then low bound");
     PG_RETURN_NULL();
  } else {
     std::uniform_int_distribution<> dist(low, high);
     return _gen_rand(fcinfo, dist, 2);
  }
}
