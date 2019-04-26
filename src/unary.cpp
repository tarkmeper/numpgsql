extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type.h"
	#include "utils/array.h"
	#include "fmgr.h"
	#include "math.h"
}
#include "array.h"

#include <functional>
#include <tuple>

// Template to apply operation to all elements in the vector. Takes the appropriate function and
// applies it to every element in the array.
template< class T, T(*FNC)(const T&) > static Datum _apply(PG_FUNCTION_ARGS, ArrayType* val_a) {
	const auto val = to_c_array<T>( val_a );

	ArrayType* res_a = PG_GETARG_ARRAYTYPE_P_COPY(0);
	const T* v = std::get<0>(val);
	T* res = (T*) ARR_DATA_PTR(res_a);
	const unsigned int elemn = std::get<2>(val);
	for (unsigned int i = 0; i < elemn; ++i ) {
		res[i] = FNC(v[i]);
	}
	PG_RETURN_POINTER(res_a);
}

//Helper function to encapsulate the boiler plate code needed on each function definition.  This is pretty ugly
//not sure if there is a better way to specificy this using templates that would also allow
//for potentially inlining.
#define UNARY_FNC(NAME, OP)		\
	extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); }	\
	template<typename T> inline static T NAME ## _op(const T& v) { return (OP); }	\
	Datum NAME(PG_FUNCTION_ARGS) {						\
	ArrayType* val = PG_GETARG_ARRAYTYPE_P(0);				\
	Oid        type  = ARR_ELEMTYPE(val);					\
	switch(type) {								\
		case FLOAT4OID:	return _apply<float, NAME ## _op<float> >(fcinfo, val);		\
		case FLOAT8OID:	return _apply<double, NAME ## _op<double> >(fcinfo, val);	\
		case INT2OID:	return _apply<short, NAME ## _op<short> >(fcinfo, val);		\
		case INT4OID:	return _apply<int, NAME ## _op<int> >(fcinfo, val);		\
		case INT8OID:	return _apply<long, NAME ## _op<long> >(fcinfo, val);		\
		default:							\
			elog(ERROR, "Unsupported array type");			\
			PG_RETURN_NULL();					\
		}								\
	}									\



UNARY_FNC(negate, ( v * -1) );


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

//Hyperbolic trig
UNARY_FNC(sinh_v, sinh( (v) ) );
UNARY_FNC(cosh_v, cosh( (v) ) );
UNARY_FNC(tanh_v, tanh( (v) ) );
UNARY_FNC(arcsinh_v, asinh( (v) ) );
UNARY_FNC(arccosh_v, acosh( (v) ) );
UNARY_FNC(arctanh_v, atanh( (v) ) );

//Rounding functions
UNARY_FNC(rint_v, rint( v ) );
UNARY_FNC(floor_v, floor( v ) );
UNARY_FNC(ceil_v, ceil( v ) );
UNARY_FNC(trunc_v, trunc( v ) );

//Exponential Functions
UNARY_FNC(exp_v, exp( v ) );
UNARY_FNC(expm1_v, (exp(v) - 1) );
UNARY_FNC(exp2_v, exp2( v ) );
UNARY_FNC(log_v, log( v ) );
UNARY_FNC(log10_v, log10( v ) );
UNARY_FNC(log2_v, log2( v ) );
UNARY_FNC(log1p_v, (log( v ) + 1) );
