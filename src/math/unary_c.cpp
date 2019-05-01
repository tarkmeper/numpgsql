extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type.h"
	#include "utils/array.h"
	#include "fmgr.h"
}
#include "../array.h"

#include <functional>
#include <tuple>

// Template to apply operation to all elements in the vector. Takes the fully defined functor
// and applies it to every element in the matrix/vector.
template<template <typename X> class F, class T> static Datum _apply(PG_FUNCTION_ARGS, ArrayType* val_a) {
	T* val = (T*)ARR_DATA_PTR( val_a );

	const T c = get_param<T>(fcinfo, 1);
	auto fnc = std::bind2nd( F<T>(), c );

	//Error checks - to add once needed
	if ( false ) {
		//elog(ERROR, "Broadcasting not supported, dimensions must match for operation");
		PG_RETURN_NULL();
	} else {
                const size_t n = arr_cnt(val_a);
		for (unsigned int i = 0; i < n; ++i ) {
			val[i] = fnc(val[i]);
		}
		PG_RETURN_POINTER(val_a);
	}
}

//Instantiate all the supported numeric variants for this object. Take in a template
//name and instantiate that functor for each type of array.
template< template <typename X> class F > static Datum _op(PG_FUNCTION_ARGS) {
	ArrayType* val = PG_GETARG_ARRAYTYPE_P_COPY(0);
	Oid        type  = ARR_ELEMTYPE(val);

	Oid ctype = get_fn_expr_argtype(fcinfo->flinfo, 1);
	if (type != ctype ) {
		elog(ERROR, "Unary constant functions require a constant of the same type as the array we got %d and %d", type, ctype);
		PG_RETURN_NULL();
	}

	switch(type) {
		case FLOAT4OID:	return _apply<F, float>(fcinfo, val);
		case FLOAT8OID:	return _apply<F, double>(fcinfo, val);
		case INT2OID:	return _apply<F, short>(fcinfo, val);
		case INT4OID:	return _apply<F, int>(fcinfo, val);
		case INT8OID:	return _apply<F, long>(fcinfo, val);
		default:
			elog(ERROR, "Unsupported array type");
			PG_RETURN_NULL();
	}
}

//Helper function to encapsulate the boiler plate code needed on each function definition.  This is pretty ugly
//not sure if there is a better way to specificy this.
#define UNARY_CONST_FNC(NAME, OP)		\
	extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); }			\
	Datum NAME(PG_FUNCTION_ARGS) { return _op<OP>(fcinfo); }


// All unary functions have two variables defined at this scope. "v" which is the value in the array and
// "c" which is the constant value passed in.
//Min/Max are special as they are polymorphic functions not functors.  There may be a better way
//to wrap them I don't know what it is.
template <class F> struct _max : std::binary_function<F, F, F> { F operator()(const F& a, const F& b) const { return std::max(a,b); } };
template <class F> struct _min : std::binary_function<F, F, F> { F operator()(const F& a, const F& b) const { return std::min(a,b); } };
UNARY_CONST_FNC(minimum_c, _min );
UNARY_CONST_FNC(maximum_c, _max );

UNARY_CONST_FNC(plus_c, std::plus );
UNARY_CONST_FNC(multiply_c, std::multiplies );
UNARY_CONST_FNC(minus_c, std::minus );
UNARY_CONST_FNC(divide_c, std::divides );

////////////////////////////////////////////////////////////////////////////////
//Clip C is a special case because the operator takes two parameters so normal simplified templates and
//macro will not work.
///////////////////////
extern "C" { PG_FUNCTION_INFO_V1(clip_c); Datum clip_c(PG_FUNCTION_ARGS); }

/*template<typename T> struct clip_c_op {
	T l, h;
	clip_c_op(const T& l_in, const T& h_in) : l(l_in), h(h_in) {}
	T operator()(const T& v) const { return std::max(l, std::min(h, v)); }
	typedef T result_type;
};*/

Datum clip_c(PG_FUNCTION_ARGS)  {
	ArrayType* val = PG_GETARG_ARRAYTYPE_P(0);
	Oid        type  = ARR_ELEMTYPE(val);

	Oid ltype = get_fn_expr_argtype(fcinfo->flinfo, 1);
	Oid htype = get_fn_expr_argtype(fcinfo->flinfo, 2);
	if (type != ltype || type != htype ) {
		elog(ERROR, "Unary constant functions require a constant of the same type as the array");
		PG_RETURN_NULL();
	}

/*	if (type == FLOAT4OID) {
		float l = PG_GETARG_FLOAT4(1);
		float h = PG_GETARG_FLOAT4(2);
		return _apply(fcinfo, val, clip_c_op<float>(l, h));
	} else {*/
		elog(ERROR, "Unsupported array type");
		PG_RETURN_NULL();
//	}
}
