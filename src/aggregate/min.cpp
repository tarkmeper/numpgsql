#include "../agg.hpp"
#include <boost/accumulators/statistics/min.hpp>

ARRAY_AGGREGATE_FNC(amin, tag::min);
SQL_AGGREGATE_FNC(v_min, tag::min);
