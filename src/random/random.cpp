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
