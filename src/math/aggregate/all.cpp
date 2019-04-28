#include "../agg.hpp"
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>


//This is a bit magic.  See boost documentation for explanation.  Key function is just the operator()
//which defines how this works.
namespace boost {
namespace accumulators {

  namespace impl {

     template<class T>
     struct AnySample {
         typedef bool result_type;
         AnySample() { v = true; }
         void op(const T& s) { v &= (s != 0); }
         result_type v;
     };

     template<class T>
     struct AnySample<std::vector<T>> {
         typedef std::vector<bool> result_type;
         AnySample() { }
         void op(const std::vector<T>& s) {
            if (v.empty()) {
               v.resize(s.size(), true);
            } else {
               for (size_t i = 0; i < s.size(); ++i) {
                  v[i] = v[i] & (s[i] != 0);
               }
            }
         }
         result_type v;
     };

     template<typename Sample>
     struct any_accumulator : accumulator_base {
        typedef typename AnySample<Sample>::result_type result_type;

        template<typename Args>                 // The constructor takes an argument pack.
        any_accumulator(Args const & args ): any( AnySample<Sample>() ) {}

        template<typename Args>
        void operator ()(Args const & args) {
           this->any.op(args[sample]);
        }

        result_type result(dont_care) const     // The result function will also be passed
        {                                       // an argument pack, but we don't use it here,
            return this->any.v;                   // so we use "dont_care" as the argument type.
        }
        private:
          AnySample<Sample> any;
    };
  }

  namespace tag {
    struct any : depends_on <>{
      typedef accumulators::impl::any_accumulator< mpl::_1 > impl;
    };
  }

  namespace extract {
     extractor< tag::any > const any = {};
  }
  using extract::any;
}}

ARRAY_AGGREGATE_FNC(aall, tag::any);
SQL_AGGREGATE_FNC(v_all, tag::any);
