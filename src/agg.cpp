//TODO - compilation is slow and produces large binaries due to the debug symbols from BOOST.  Determine
//if tehre is some way to disable those symbols.
extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type_d.h"
	#include "utils/array.h"
	#include "fmgr.h"

}
#include "array.h"

#include <algorithm>
#include <functional>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/skewness.hpp>
#include <boost/accumulators/statistics/kurtosis.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/variance.hpp>


using namespace boost::accumulators;

//Templated functions to switch return basedo on data type. This allows the various aggregate
//functions to return the appropriate data element.
template<class T> Datum _ret(const T& val) { T::unimplemented_function; return Datum(); }
template<> Datum _ret<double>(const double& val) { PG_RETURN_FLOAT8(val); }
template<> Datum _ret<float>(const float& val) { PG_RETURN_FLOAT4(val); }
template<> Datum _ret<short>(const short& val) { PG_RETURN_INT16(val); }
template<> Datum _ret<int>(const int& val) { PG_RETURN_INT32(val); }
template<> Datum _ret<long>(const long& val) { PG_RETURN_INT64(val); }

//Create an aceclerator for the target function and apply on the dataset.
template<class V, class T> auto _apply(T* ptr, size_t n) {
  accumulator_set<T, features<V> > acc;
  std::for_each(ptr, ptr + n, std::ref(acc));
  return acc;
}

//Define a function for each aggregation which internally has a switch between the basic types.
//This is needed.  Todo - investigate if it is possible to generate more of this using C++ templates
//where the debuging is slightly easier then C macros.
#define STATISTICS_FNC(NAME, OP)  \
  extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); } \
  Datum NAME(PG_FUNCTION_ARGS) { \
    ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0); \
    Oid type  = ARR_ELEMTYPE(arr); \
    const size_t n = arr_cnt(arr); \
    void* ptr = ARR_DATA_PTR(arr); \
    switch(type) { \
      case FLOAT4OID: return _ret( OP( _apply<tag::OP >( (float4*)ptr, n))); \
      case FLOAT8OID: return _ret( OP( _apply<tag::OP >( (float8*)ptr, n))); \
      case INT2OID: return _ret( OP( _apply<tag::OP >( (int16*)ptr, n))); \
      case INT4OID: return _ret( OP( _apply<tag::OP >( (int32*)ptr, n))); \
      case INT8OID: return _ret( OP( _apply<tag::OP >( (int64*)ptr, n))); \
      default:                                                        \
        elog(ERROR, "Unsupported array type"); \
        PG_RETURN_NULL(); \
    } \
  }

STATISTICS_FNC(amean, mean);
STATISTICS_FNC(avariance, variance);
STATISTICS_FNC(askew, skewness);
STATISTICS_FNC(akurtosis, kurtosis);

STATISTICS_FNC(amax, max);
STATISTICS_FNC(amin, min);
