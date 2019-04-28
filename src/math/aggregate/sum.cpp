#include "../agg.hpp"
#include <boost/accumulators/statistics/sum.hpp>

ARRAY_AGGREGATE_FNC(asum, tag::sum);
SQL_AGGREGATE_FNC(v_sum, tag::sum);
