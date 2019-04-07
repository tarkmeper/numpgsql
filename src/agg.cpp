extern "C"  {
	#include "postgres.h"
	#include "catalog/pg_type_d.h"
	#include "utils/array.h"
	#include "fmgr.h"

	PG_FUNCTION_INFO_V1(mean); Datum mean(PG_FUNCTION_ARGS);

	PG_FUNCTION_INFO_V1(stdev); Datum stdev(PG_FUNCTION_ARGS);
}
#include "array.h"

#include <functional>
#include <tuple>

/***** operator template ****/
Datum mean(PG_FUNCTION_ARGS) {
	ArrayType* val_a = PG_GETARG_ARRAYTYPE_P(0);

	const auto val = to_c_array<float>( val_a );

	//Error checks - to add once needed
	if ( false ) {
		//elog(ERROR, "Broadcasting not supported, dimensions must match for operation");
		PG_RETURN_NULL();
	} else {
		const float* v = std::get<0>(val);
		float result = 0.0;
		for (unsigned int i = 0; i < std::get<2>(val); ++i ) {
			result += v[i];
		}
		PG_RETURN_FLOAT4(result / std::get<2>(val));
	}
}

Datum stdev(PG_FUNCTION_ARGS) {
	ArrayType* val_a = PG_GETARG_ARRAYTYPE_P(0);

	const auto val = to_c_array<float>( val_a );

	//Error checks - to add once needed
	if ( std::get<2>(val) < 2 ) {
		elog(ERROR, "Insufficient elements for calculation of stdev");
		PG_RETURN_NULL();
	} else {
		const float* v = std::get<0>(val);
		//this is not particularly robust as floating point errors 
		//can accumulate.
		float sum = 0.0, sum_sq = 0.0;
		for (unsigned int i = 0; i < std::get<2>(val); ++i ) {
			sum += v[i];
			sum_sq += (v[i] * v[i]);
		}
		const float size = (float)(std::get<2>(val));
		const float mean = sum / size;
		const float stdev =  sum_sq / size - mean * mean;
		PG_RETURN_FLOAT4( stdev );
	}
}
