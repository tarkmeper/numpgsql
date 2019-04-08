extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type_d.h"
	#include "utils/array.h"
	#include "fmgr.h"
}
#include "array.h"

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

template<class T> T get_param(PG_FUNCTION_ARGS, int p)  { T::unimplemented_function; }
template<> float get_param<float>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_FLOAT4(p); }
template<> double get_param<double>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_FLOAT8(p); }
template<> short get_param<short>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_INT16(p); }
template<> int get_param<int>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_INT32(p); }
template<> long get_param<long>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_INT64(p); }


//Helper template to automate getting the required parameter.
template< template <typename X> class F, class T> static Datum _op2(PG_FUNCTION_ARGS, ArrayType* val) {
	const T c = get_param<T>(fcinfo, 1);
	return _apply(fcinfo, val, F<T>(c));
}


//Instantiate all the supported numeric variants for this object. Take in a template
//name and instantiate that functor for each type of array.
template< template <typename X> class F > static Datum _op(PG_FUNCTION_ARGS) {
	ArrayType* val = PG_GETARG_ARRAYTYPE_P(0);
	Oid        type  = ARR_ELEMTYPE(val);

	Oid ctype = get_fn_expr_argtype(fcinfo->flinfo, 1);
	if (type != ctype ) {
		elog(ERROR, "Unary constant functions require a constant of the same type as the array we got %d and %d", type, ctype);
		PG_RETURN_NULL();
	}

	switch(type) {
		case FLOAT4OID:	return _op2<F, float>(fcinfo, val);
		case FLOAT8OID:	return _op2<F, double>(fcinfo, val);
		case INT2OID:	return _op2<F, short>(fcinfo, val);
		case INT4OID:	return _op2<F, int>(fcinfo, val);
		case INT8OID:	return _op2<F, long>(fcinfo, val);
		default:
			elog(ERROR, "Unsupported array type");
			PG_RETURN_NULL();
	}
}

//Helper function to encapsulate the boiler plate code needed on each function definition.  This is pretty ugly
//not sure if there is a better way to specificy this.
#define UNARY_CONST_FNC(NAME, OP)		\
	extern "C" { PG_FUNCTION_INFO_V1(NAME); Datum NAME(PG_FUNCTION_ARGS); }			\
	template<typename T> struct NAME ## _op{						\
		T c;										\
		NAME ## _op (const T& v) : c(v) {}						\
		T operator()(const T& v) const { return (OP); }; typedef T result_type; };	\
	Datum NAME(PG_FUNCTION_ARGS) { return _op<NAME ## _op>(fcinfo); }


// All unary functions have two variables defined at this scope. "v" which is the value in the array and
// "c" which is the constant value passed in.
UNARY_CONST_FNC(minimum_c, (std::min(v, c)) );
UNARY_CONST_FNC(maximum_c, (std::max(v, c)) );
UNARY_CONST_FNC(plus_c, (v + c) );
UNARY_CONST_FNC(multiply_c, (v * c) );

//Clip C is a special case because the operator takes two parameters so normal simplified templates and
//macro will not work.
extern "C" { PG_FUNCTION_INFO_V1(clip_c); Datum clip_c(PG_FUNCTION_ARGS); }

template<typename T> struct clip_c_op {
	T l, h;
	clip_c_op(const T& l_in, const T& h_in) : l(l_in), h(h_in) {}
	T operator()(const T& v) const { return std::max(l, std::min(h, v)); }
	typedef T result_type;
};

Datum clip_c(PG_FUNCTION_ARGS)  {
	ArrayType* val = PG_GETARG_ARRAYTYPE_P(0);
	Oid        type  = ARR_ELEMTYPE(val);

	Oid ltype = get_fn_expr_argtype(fcinfo->flinfo, 1);
	Oid htype = get_fn_expr_argtype(fcinfo->flinfo, 2);
	if (type != ltype || type != htype ) {
		elog(ERROR, "Unary constant functions require a constant of the same type as the array");
		PG_RETURN_NULL();
	}

	if (type == FLOAT4OID) { 
		float l = PG_GETARG_FLOAT4(1);
		float h = PG_GETARG_FLOAT4(2);
		return _apply(fcinfo, val, clip_c_op<float>(l, h));
	} else {
		elog(ERROR, "Unsupported array type");
		PG_RETURN_NULL();
	}
}


