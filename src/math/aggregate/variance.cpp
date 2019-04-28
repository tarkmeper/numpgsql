#include "../agg.hpp"
#include <boost/accumulators/statistics/variance.hpp>

ARRAY_AGGREGATE_FNC(avariance, tag::variance);
SQL_AGGREGATE_FNC(v_variance, tag::variance);
