#include "../agg.hpp"
#include <cmath>

namespace std {
  template<class T> vector<T> sqrt(vector<T> val) {
    vector<T> res(val.size());
    for (size_t i = 0; i < val.size(); ++i) {
       res[i] = sqrt(val[i]);
    }
    return res;
  }
}

#include <boost/accumulators/statistics/skewness.hpp>

ARRAY_AGGREGATE_FNC(askew, tag::skewness);
SQL_AGGREGATE_DBL_ONLY_FNC(v_skew, tag::skewness);
