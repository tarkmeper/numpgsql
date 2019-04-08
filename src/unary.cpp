extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type_d.h"
	#include "utils/array.h"
	#include "fmgr.h"
}
#include "array.h"
#include <math.h>

#include <functional>
#include <tuple>

// Template to apply operation to all elements in the vector. Takes the fully defined functor
// and applies it to every element in the matrix/vector.
template<class F> static Datum _apply(PG_FUNCTION_ARGS, ArrayType* val_a, const F& fnc) {
	typedef typename F::result_type T;
	const auto val = to_c_array<T>( val_a );

	//Error checks - to add once needed
	if ( false ) {
		//elog(ERROR, "Broadcasting not supported, dimensions must match for operation");
		PG_RETURN_NULL();
	} else {
		ArrayType* res_a = PG_GETARG_ARRAYTYPE_P_COPY(0);
		const T* v = std::get<0>(val);
		T* res = (T*) ARR_DATA_PTR(res_a);
		for (unsigned int i = 0; i < std::get<2>(val); ++i ) {
			res[i] = fnc(v[i]);
		}
		PG_RETURN_POINTER(res_a);
	}
}

//Instantiate all the supported numeric variants for this object. Take in a template
//name and instantiate that functor for each type of array.
template< template <typename T> class F > static Datum _op(PG_FUNCTION_ARGS) {
	ArrayType* val = PG_GETARG_ARRAYTYPE_P(0);
	Oid        type  = ARR_ELEMTYPE(val);
	switch(type) {
		case FLOAT4OID:	return _apply(fcinfo, val, F<float>());
		case FLOAT8OID: return _apply(fcinfo, val, F<double>());
		case INT2OID: return   _apply(fcinfo, val, F<short>());
		case INT4OID: return   _apply(fcinfo, val, F<int>());
		case INT8OID: return   _apply(fcinfo, val, F<long>());
		default:
			elog(ERROR, "Unsupported array type");
			PG_RETURN_NULL();
	}
}

//Helper function to encapsulate the boiler plate code needed on each function definition.  This is pretty ugly
//not sure if there is a better way to specificy this.
#define UNARY_FNC(NAME, OP)		\
	extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); }	\
	template<typename T> struct NAME ## _op{ T operator()(const T& v) const { return (OP); }; typedef T result_type; };	\
	Datum NAME(PG_FUNCTION_ARGS) { return _op<NAME ## _op>(fcinfo); }	\

UNARY_FNC(negate, (-1 * v));

//Trig functions
UNARY_FNC(sin_v, sin( (v) ) );
UNARY_FNC(cos_v, cos( (v) ) );
UNARY_FNC(tan_v, tan( (v) ) );
UNARY_FNC(arcsin_v, asin( (v) ) );
UNARY_FNC(arccos_v, acos( (v) ) );
UNARY_FNC(arctan_v, atan( (v) ) );
UNARY_FNC(degrees_v, ( 180 * v / M_PI ) );
UNARY_FNC(radians_v, ( v * M_PI / 180 ) );
UNARY_FNC(unwrap_v, fmod(v, 2* M_PI)  );

