#include "agg.hpp"

extern "C" {
  PG_FUNCTION_INFO_V1(internal_to_array);
  Datum internal_to_array(PG_FUNCTION_ARGS);
}


Datum internal_to_array(PG_FUNCTION_ARGS) {
  //TODO trigger destructor
  Assert(AggCheckCallContext(fcinfo, NULL));
  if (PG_ARGISNULL(0)) {
     PG_RETURN_NULL();
  } else {
     AggInternal* state = (AggInternal*)(PG_GETARG_POINTER(0));
     PG_RETURN_ARRAYTYPE_P( state->fnc(state)  );
  }
}

