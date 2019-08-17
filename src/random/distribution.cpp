extern "C"  {
        #include "postgres.h"
        #include "catalog/pg_type.h"
        #include "fmgr.h"
}
#include "random.h"
#include "../array.h"

#include <boost/random.hpp>

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

#define RAND_FUNCTION(NAME) \
	extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); } \
        Datum NAME(PG_FUNCTION_ARGS)

RAND_FUNCTION(gen_rand) {
  std::uniform_real_distribution<double> dist{0.0, 1.0};
  return _gen_rand(fcinfo, dist);
}


RAND_FUNCTION(gen_randn) {
  //for soem reason can't determine "std' variant is producing warnings.
  std::normal_distribution<double> dist{0.0, 1.0};
  return _gen_rand(fcinfo, dist);
}


RAND_FUNCTION(gen_randint) {
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

RAND_FUNCTION(gen_beta) {
  float alpha = get_param<double>(fcinfo, 0);
  float beta  = get_param<double>(fcinfo, 1);
  if (alpha < 0 || beta < 0){
     elog(ERROR, "BETA: Can't have negative alpha/beta parameters");
     PG_RETURN_NULL();
  } else {
     boost::random::beta_distribution<double> dist(alpha, beta);
     return _gen_rand(fcinfo, dist, 2);
  }
}

RAND_FUNCTION(gen_binomial) {
  int t = get_param<int>(fcinfo, 0);
  float p  = get_param<double>(fcinfo, 1);
  if (t < 0 || p < 0 || p > 1){
     elog(ERROR, "BINOMIAL: Input parameters outside of range");
     PG_RETURN_NULL();
  } else {
     boost::random::binomial_distribution<int> dist(t,p);
     return _gen_rand(fcinfo, dist, 2);
  }
}

