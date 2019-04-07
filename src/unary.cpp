extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type_d.h"
	#include "utils/array.h"
	#include "fmgr.h"

	PG_FUNCTION_INFO_V1(negate); Datum negate(PG_FUNCTION_ARGS);

	PG_FUNCTION_INFO_V1(clip_c); Datum clip_c(PG_FUNCTION_ARGS);

	PG_FUNCTION_INFO_V1(minimum_c); Datum minimum_c(PG_FUNCTION_ARGS);
	PG_FUNCTION_INFO_V1(maximum_c); Datum maximum_c(PG_FUNCTION_ARGS);

	PG_FUNCTION_INFO_V1(plus_c); Datum plus_c(PG_FUNCTION_ARGS);
	PG_FUNCTION_INFO_V1(multiply_c); Datum plus_c(PG_FUNCTION_ARGS);

	PG_FUNCTION_INFO_V1(greater_c); Datum plus_c(PG_FUNCTION_ARGS);

}
#include "array.h"

#include <functional>
#include <tuple>

/***** operator template ****/
template<class T, typename FNC> static Datum _op(PG_FUNCTION_ARGS, const FNC& fnc) {
	ArrayType* val_a = PG_GETARG_ARRAYTYPE_P(0);

	const auto val = to_c_array<T>( val_a );

	//Error checks - to add once needed
	if ( false ) {
		//elog(ERROR, "Broadcasting not supported, dimensions must match for operation");
		PG_RETURN_NULL();
	} else {
		ArrayType* res_a = PG_GETARG_ARRAYTYPE_P_COPY(0);
		const float* v = std::get<0>(val);
		float* res = (T*) ARR_DATA_PTR(res_a);
		for (unsigned int i = 0; i < std::get<2>(val); ++i ) {
			res[i] = fnc(v[i]);
		}
		PG_RETURN_POINTER(res_a);
	}
}

template<class T, typename FNC> static Datum _op_1param(PG_FUNCTION_ARGS, const FNC& fnc) {
	const float v = PG_GETARG_FLOAT4(1);
	return _op<float>(fcinfo, [=](float f) { return fnc(f,v); });
}




Datum negate(PG_FUNCTION_ARGS) { return _op<float>(fcinfo, [](float f) { return -1 * f; }); }

Datum clip_c(PG_FUNCTION_ARGS)  {
                                   const float min_val = PG_GETARG_FLOAT4(1);
                                   const float max_val = PG_GETARG_FLOAT4(2);
                                   return _op<float>(fcinfo, [=](float f) { return std::max(min_val, std::min(max_val, f)); });
				}


Datum minimum_c(PG_FUNCTION_ARGS)  { return _op_1param<float>(fcinfo, [=](float f, float c) { return std::min(f, c); }); }
Datum maximum_c(PG_FUNCTION_ARGS)  { return _op_1param<float>(fcinfo, [=](float f, float c) { return std::max(f, c); }); }

Datum plus_c(PG_FUNCTION_ARGS)  { return _op_1param<float>(fcinfo, [=](float f, float c) { return f + c; }); }
Datum multiply_c(PG_FUNCTION_ARGS)  { return _op_1param<float>(fcinfo, [=](float f, float c) { return f * c; }); }


Datum greater_c(PG_FUNCTION_ARGS)  {
	ArrayType* val_a = PG_GETARG_ARRAYTYPE_P(0);
	const float c = PG_GETARG_FLOAT4(1);

	const auto val = to_c_array<float>( val_a );
	const float* v = std::get<0>(val);

	Datum* res = (Datum*)palloc(sizeof(Datum) * std::get<2>(val));
	for (unsigned int i = 0; i < std::get<2>(val); ++i ) {
		res[i] = BoolGetDatum(v[i] > c);
	}
	ArrayType* result = construct_array(res, std::get<2>(val), BOOLOID, 1, true, 'b');
	PG_RETURN_ARRAYTYPE_P(result);
}
