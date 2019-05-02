extern "C"  {
        #include "postgres.h"
        #include "catalog/pg_type.h"
        #include "utils/array.h"
        #include "fmgr.h"
}
#include <vector>
#include <tuple>


inline static size_t arr_cnt(const ArrayType* arr) {
	size_t result = 1;
	for (int i = 0; i < ARR_NDIM(arr); ++i){
		result *= ARR_DIMS(arr)[i];
	}
	return result;
}

//Return values - TODO rename function this was poorly named.
template<class T> inline Datum _ret(const T& val) { T::unimplemented_function; return Datum(); }
template<> inline Datum _ret<double>(const double& val) { PG_RETURN_FLOAT8(val); }
template<> inline Datum _ret<float>(const float& val) { PG_RETURN_FLOAT4(val); }
template<> inline Datum _ret<short>(const short& val) { PG_RETURN_INT16(val); }
template<> inline Datum _ret<int>(const int& val) { PG_RETURN_INT32(val); }
template<> inline Datum _ret<long>(const long& val) { PG_RETURN_INT64(val); }
template<> inline Datum _ret<bool>(const bool& val) { PG_RETURN_BOOL(val); }

//OID types
template<class T> inline Oid oid_type() { return T::unimplemented_feature; }
template<> inline Oid oid_type<float>() { return FLOAT4OID; }
template<> inline Oid oid_type<double>() { return FLOAT8OID; }
template<> inline Oid oid_type<short>() { return INT2OID; }
template<> inline Oid oid_type<int>() { return INT4OID; }
template<> inline Oid oid_type<long>() { return INT8OID; }
template<> inline Oid oid_type<bool>() { return BOOLOID; }

//Alignment codes
template<class T> char oid_align() { return T::unimplemented_feature; }
template<> inline char oid_align<float>() { return 'i'; }
template<> inline char oid_align<double>() { return 'd'; }
template<> inline char oid_align<short>() { return 's'; }
template<> inline char oid_align<int>() { return 'i'; }
template<> inline char oid_align<long>() { return 'd'; }
template<> inline char oid_align<bool>() { return 'c'; }

//Get param values
template<class T> inline T get_param(PG_FUNCTION_ARGS, int p)  { T::unimplemented_function; }
template<> inline bool get_param<bool>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_BOOL(p); }
template<> inline float get_param<float>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_FLOAT4(p); }
template<> inline double get_param<double>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_FLOAT8(p); }
template<> inline short get_param<short>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_INT16(p); }
template<> inline int get_param<int>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_INT32(p); }
template<> inline long get_param<long>(PG_FUNCTION_ARGS, int p) { return PG_GETARG_INT64(p); }
