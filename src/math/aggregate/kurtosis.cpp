#include "../agg.hpp"
#include <boost/accumulators/statistics/kurtosis.hpp>

ARRAY_AGGREGATE_FNC(akurtosis, tag::kurtosis);
//SQL_AGGREGATE_DBL_ONLY_FNC(v_kurtosis, tag::kurtosis);
