extern "C"  {
        #include "postgres.h"
        #include "catalog/pg_type_d.h"
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

template<class T> inline Datum _ret(const T& val) { T::unimplemented_function; return Datum(); }
template<> inline Datum _ret<double>(const double& val) { PG_RETURN_FLOAT8(val); }
template<> inline Datum _ret<float>(const float& val) { PG_RETURN_FLOAT4(val); }
template<> inline Datum _ret<short>(const short& val) { PG_RETURN_INT16(val); }
template<> inline Datum _ret<int>(const int& val) { PG_RETURN_INT32(val); }
template<> inline Datum _ret<long>(const long& val) { PG_RETURN_INT64(val); }
template<> inline Datum _ret<bool>(const bool& val) { PG_RETURN_BOOL(val); }

template<class T> inline Oid oid_type() { return T::unimplemented_feature; }
template<> inline Oid oid_type<float>() { return FLOAT4OID; }
template<> inline Oid oid_type<double>() { return FLOAT8OID; }
template<> inline Oid oid_type<short>() { return INT2OID; }
template<> inline Oid oid_type<int>() { return INT4OID; }
template<> inline Oid oid_type<long>() { return INT8OID; }
template<> inline Oid oid_type<bool>() { return BOOLOID; }

template<class T> char oid_align() { return T::unimplemented_feature; }
template<> inline char oid_align<float>() { return 'i'; }
template<> inline char oid_align<double>() { return 'd'; }
template<> inline char oid_align<short>() { return 's'; }
template<> inline char oid_align<int>() { return 'i'; }
template<> inline char oid_align<long>() { return 'd'; }
template<> inline char oid_align<bool>() { return 'c'; }

template<class T> std::tuple<T*, std::vector<unsigned int>, unsigned int > to_c_array(ArrayType* arr) {
	T * ptr = (T*)ARR_DATA_PTR(arr);
	std::vector<unsigned int> dims;
	unsigned int prod = 0;

	if (ARR_NDIM(arr) == 0 ) { 
		prod = 0;
	} else {
		prod = 1;
		for (int i = 0; i < ARR_NDIM(arr); ++i) {
			dims.push_back( ARR_DIMS(arr)[i] );
			prod *= dims.back();
		}
	}
	return std::make_tuple(ptr, dims, prod);
}
