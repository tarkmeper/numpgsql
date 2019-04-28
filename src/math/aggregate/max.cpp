#include "../agg.hpp"
#include <boost/accumulators/statistics/max.hpp>

ARRAY_AGGREGATE_FNC(amax, tag::max);
SQL_AGGREGATE_FNC(v_max, tag::max);
