#include "../agg.hpp"
#include <boost/accumulators/statistics/mean.hpp>

ARRAY_AGGREGATE_FNC(amean, tag::mean);
SQL_AGGREGATE_FNC(v_mean, tag::mean);
