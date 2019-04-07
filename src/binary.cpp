extern "C"  {
	#include "postgres.h"
	#include "utils/array.h"
	#include "fmgr.h"

	PG_FUNCTION_INFO_V1(add); Datum add(PG_FUNCTION_ARGS);
	PG_FUNCTION_INFO_V1(minus); Datum minus(PG_FUNCTION_ARGS);
	PG_FUNCTION_INFO_V1(multiplies); Datum multiplies(PG_FUNCTION_ARGS);


	PG_FUNCTION_INFO_V1(multiplies_ab); Datum multiplies_ab(PG_FUNCTION_ARGS);

}
#include "array.h"

#include <functional>
#include <tuple>

/***** ADD ****/
template<class L, class R> static Datum _op(PG_FUNCTION_ARGS, std::function<L(L,R)> fnc) {
	ArrayType* left_a = PG_GETARG_ARRAYTYPE_P(0);
	ArrayType* right_a = PG_GETARG_ARRAYTYPE_P(1);

	const auto left = to_c_array<L>( left_a );
	const auto right = to_c_array<R>( right_a );

	//Error checks
	if ( std::get<1>(left) != std::get<1>(right) ) {
		if (std::get<2>(left) == 0) {
			PG_RETURN_POINTER(right_a);
		} else if (std::get<2>(right) == 0) {
			PG_RETURN_POINTER(left_a);
		} else {
			elog(ERROR, "Broadcasting not supported, dimensions must match for operation.");
		}
		PG_RETURN_NULL();
	} else {
		ArrayType* res_a = PG_GETARG_ARRAYTYPE_P_COPY(0);
		const L* l = std::get<0>(left);
		const R* r = std::get<0>(right);
		L* res = (L*) ARR_DATA_PTR(res_a);
		for (unsigned int i = 0; i < std::get<2>(left); ++i ) {
			res[i] = fnc(l[i], r[i]);
		}
		PG_RETURN_POINTER(res_a);
	}
}


Datum add(PG_FUNCTION_ARGS) { return _op<float, float>(fcinfo, [](float l, float r) { return r + l; } ); }
Datum minus(PG_FUNCTION_ARGS) { return _op<float, float>(fcinfo, [](float l, float r) { return l - r; } ); }
Datum multiplies(PG_FUNCTION_ARGS) { return _op<float, float>(fcinfo, [](float l, float r) { return r * l; } ); }

Datum multiplies_ab(PG_FUNCTION_ARGS) { return _op<float, bool>(fcinfo, [](float l, bool r) { return r ? l : 0; } ); }

