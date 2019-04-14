#include "utils/array.h"
#include <vector>
#include <tuple>


inline static size_t arr_cnt(const ArrayType* arr) {
	size_t result = 1;
	for (int i = 0; i < ARR_NDIM(arr); ++i){
		result *= ARR_DIMS(arr)[i];
	}
	return result;
}


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
