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
  text  *arg1 = PG_GETARG_TEXT_PP(0);
  std::string val(VARDATA(arg1));
  std::istringstream ss(val);
  ss >> rgen;

  PG_RETURN_NULL();
}


std::uniform_real_distribution<double> dist{0.0, 1.0};
extern "C" { PG_FUNCTION_INFO_V1(gen_rand); Datum gen_rand(PG_FUNCTION_ARGS); }
Datum gen_rand(PG_FUNCTION_ARGS) {
  ArrayType* arr = PG_GETARG_ARRAYTYPE_P_COPY(0);

  if (ARR_NDIM(arr) == 0 ) {
     PG_RETURN_POINTER(construct_empty_array(oid_type<double>()));
  } else if (ARR_NDIM(arr) != 1 ) {
     elog(ERROR, "GEN_RAND: Size specification can only be 1-dimension array");
     PG_RETURN_NULL();
  } else if (ARR_DIMS(arr)[0] != 1 ) {
     elog(ERROR, "GEN_RAND: Currently only support 1 dimensional arrays.");
     PG_RETURN_NULL();
  }

  const size_t n = * ((int*)(ARR_DATA_PTR(arr)));
  Datum* r = (Datum*)palloc(n * sizeof(Datum));
  for (size_t i = 0; i < n; ++i ) {
     r[i] = _ret<double>(dist(rgen));
  }

  ArrayType* res = construct_array(r, n, oid_type<double>(), 8, true, oid_align<double>());
  PG_RETURN_POINTER(res);
}



