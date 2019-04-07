extern "C"  {
	#include "postgres.h"
	#include "utils/array.h"
	#include "fmgr.h"

	PG_FUNCTION_INFO_V1(sort); Datum sort(PG_FUNCTION_ARGS);
}
#include "array.h"

#include <algorithm>
#include <tuple>

/***** operator template ****/
Datum sort(PG_FUNCTION_ARGS) {
	ArrayType* val_a = PG_GETARG_ARRAYTYPE_P_COPY(0);
	auto val = to_c_array<float>( val_a );
	float* v = std::get<0>(val);

	std::sort(v, v + std::get<2>(val));
	PG_RETURN_POINTER(val_a);
}

