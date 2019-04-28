#ifndef AGG_HPP
#define AGG_HPP
//TODO - compilation is slow and produces large binaries due to the debug symbols from BOOST.  Determine
//if tehre is some way to disable those symbols.
extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type.h"
	#include "utils/array.h"
	#include "fmgr.h"

}
#include "../array.h"

#include <algorithm>
#include <functional>
#include <boost/accumulators/numeric/functional/vector.hpp>

using namespace boost::numeric::operators;


#include <boost/accumulators/accumulators.hpp>

using namespace boost::accumulators;

//////////////////////////////////////////////////////////////////////////////////////
//  Array Aggregation Functions
//  Compute the aggregation across a single array perhaps in the future with support
//  for aggregating along an axis.
/////////////////////////////////////////////////////////////////////////////////////

//Create an aceclerator for the target function and apply on the dataset.
template<class V, class T> static Datum _apply(T* ptr, size_t n) {
  accumulator_set<T, features<V> > acc;
  std::for_each(ptr, ptr + n, std::ref(acc));
  return _ret(extract_result<V>(acc));
}

template<class V> static Datum _op(PG_FUNCTION_ARGS) {
    ArrayType* arr = PG_GETARG_ARRAYTYPE_P(0);
    Oid type  = ARR_ELEMTYPE(arr);
    const size_t n = arr_cnt(arr);
    void* ptr = ARR_DATA_PTR(arr);
    switch(type) {
      case BOOLOID: return _apply<V> ( (bool*)ptr, n);
      case FLOAT4OID: return _apply<V>( (float4*)ptr, n );
      case FLOAT8OID: return _apply<V>( (float8*)ptr, n );
      case INT2OID: return _apply<V>( (int16*)ptr, n );
      case INT4OID: return _apply<V>( (int32*)ptr, n );
      case INT8OID: return _apply<V>( (int64*)ptr, n );
      default:
        elog(ERROR, "Unsupported array type");
        PG_RETURN_NULL();
    }
}


//Define a function for each aggregation which internally has a switch between the basic types.
//This is needed.  Todo - investigate if it is possible to generate more of this using C++ templates
//where the debuging is slightly easier then C macros.
#define ARRAY_AGGREGATE_FNC(NAME, OP)  \
  extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); } \
  Datum NAME(PG_FUNCTION_ARGS) { return _op<OP>(fcinfo); }


//////////////////////////////////////////////////////////////////////////////////////
//  SQL Aggregation Functions
//  Compute the aggregation across multiple arrays using an SQL group by clause.
//  Normally we would expect that all aggregartes would have both.
/////////////////////////////////////////////////////////////////////////////////////

struct AggInternal  {
  size_t cnt;
  void*  acc;
  ArrayType* (*fnc)(AggInternal*);
};

template<class V, class T> ArrayType* to_array(AggInternal* a) {
    typedef accumulator_set< std::vector<T>, features<V> > Acc_T;

    if (a->cnt == 0) {
      return construct_empty_array(FLOAT4OID);
    } else {
      Acc_T* acc = (Acc_T*)(a->acc);

      //Figure out result type automatically
      auto v = extract_result<V>(*acc);
      Datum* arr = (Datum*)palloc(a->cnt * sizeof(Datum));
      for (unsigned int i = 0; i < a->cnt; ++i) {
        arr[i] = Float4GetDatum((float)v[i]);
      }

      //Now delete the aggregator and set the value to nullptr.
      acc->~Acc_T();
      a->acc = nullptr;

      return construct_array(arr, a->cnt, FLOAT4OID, 4, true, 'i');
    }
}


//Apply takes the vector we have and assuming that the vector isn't empty and matches previous vector sizes
//will apply the accumulator for that vector.
template<class V, class T> Datum _sql_apply(MemoryContext& ctx, AggInternal* state, T* arr_ptr, const size_t n) {
  typedef accumulator_set< std::vector<T>, features<V> > Acc_T;

  if (!state) {
    //Use C memory allocator so that Postgres can manage the memory
    //critical that internal representation have trivial destructor
    //we need to force call the destructor on Acc_T.
    void *acc = MemoryContextAlloc(ctx, sizeof(Acc_T));
    acc = new (acc) Acc_T(std::vector<T>(n));

    state = (AggInternal*)MemoryContextAlloc(ctx, sizeof(AggInternal));
    state->cnt = n;
    state->acc = acc;
    state->fnc = &to_array<V,T>;
  }
  Acc_T* acc = (Acc_T*)(state->acc);

  //special case - if the array is empty allow the aggregation to continue
  //excluding it.
  if (n == 0) {
    PG_RETURN_POINTER(state);
  } else if ( n != state->cnt ) {
    elog(ERROR, "All vectors to be aggregated must have same number of elements");
    PG_RETURN_POINTER(state);
  }

  std::vector<T> v(arr_ptr, arr_ptr+n);
  (*acc)(v);
  PG_RETURN_POINTER(state);
};

template<class V, class T, bool ALL> struct Call {
  Datum operator()(MemoryContext&, AggInternal*, T* ptr, size_t n) { V::unimplimeneted_function; return Datum(); }
};

template<class V, class T> struct Call<V, T, true>{
  Datum operator()(MemoryContext& ctx, AggInternal* state, T* ptr, size_t n) {
    return _sql_apply<V>(ctx, state, ptr, n);
  }
};


template<class V, class T> struct Call<V, T, false>{
  Datum operator()(MemoryContext& ctx, AggInternal* state, T* ptr, size_t n) {
    std::vector<double> v(ptr, ptr+n);
    return _sql_apply<V>(ctx, state, v.data(), n);
  }
};



template<class V, bool ALL> static Datum _sql_op(PG_FUNCTION_ARGS) {
  MemoryContext ctx;
  AggInternal * state = nullptr;

  //Since we don't have strict on the array pointer could be
  //null
  if (PG_ARGISNULL(1)) {
     if (PG_ARGISNULL(0)) { PG_RETURN_NULL(); }
     else { PG_RETURN_POINTER(PG_GETARG_POINTER(0)); }
  }

  ArrayType* arr = PG_GETARG_ARRAYTYPE_P(1);
  Oid type  = ARR_ELEMTYPE(arr);
  const size_t n = arr_cnt(arr);
  void* ptr = ARR_DATA_PTR(arr);

  //Block calls when not in aggregation mode.
  if (!AggCheckCallContext(fcinfo, &ctx)) {
    elog(ERROR,  "Called in non-aggregate context");
    PG_RETURN_NULL();
  }

  //If we have a state variable already return that.
  if (!PG_ARGISNULL(0)) {
    state = (AggInternal*)PG_GETARG_POINTER(0);
  } else if ( n == 0 ) {
     // special case if we don't yet have a state, and the
     // size is 0 then return null
     PG_RETURN_NULL();
  }

  if (state && state->acc == nullptr) {
     elog(ERROR, "Aggregation has already been finalized - cannot continue aggregating");
     PG_RETURN_NULL();
  }

  switch(type) {
      case FLOAT4OID: return Call<V, float, ALL>()( ctx, state, (float4*)ptr, n);
      case FLOAT8OID: return Call<V, double, ALL>()( ctx, state, (float8*)ptr, n);
      case INT2OID: return Call<V, short, ALL>()( ctx, state, (int16*)ptr, n);
      case INT4OID: return Call<V, int, ALL>()( ctx, state, (int32*)ptr, n);
      case INT8OID: return Call<V, long, ALL>()( ctx, state, (int64*)ptr, n);
      default:
         elog(ERROR, "Unsupported Array Type");
         PG_RETURN_NULL();
  }
}


#define SQL_AGGREGATE_FNC(NAME, OP)  \
  extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); } \
  Datum NAME(PG_FUNCTION_ARGS) { return _sql_op<OP, true>(fcinfo); }

#define SQL_AGGREGATE_DBL_ONLY_FNC(NAME, OP)  \
  extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); } \
  Datum NAME(PG_FUNCTION_ARGS) { return _sql_op<OP, false>(fcinfo); }



#endif
