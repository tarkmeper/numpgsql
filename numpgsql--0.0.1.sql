-- This program is open source, licensed under the PostgreSQL License.
-- For license terms, see the LICENSE file.
--
-- Copyright (C) 2019: Mark Teper

\echo Use "CREATE EXTENSION numpgsql" to load this file. \quit

SET client_encoding = 'UTF8';

-- creation functions

-- unary functionss
CREATE FUNCTION negate(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'negate';

--    Trig functions
CREATE FUNCTION sin(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'sin_v';
CREATE FUNCTION cos(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'cos_v';
CREATE FUNCTION tan(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'tan_v';
CREATE FUNCTION arcsin(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'arcsin_v';
CREATE FUNCTION arccos(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'arccos_v';
CREATE FUNCTION arctan(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'arctan_v';
CREATE FUNCTION degrees(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'degrees_v';
CREATE FUNCTION radians(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'radians_v';
CREATE FUNCTION unwrap(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'unwrap_v';

-- Hyperbolic trig functions
CREATE FUNCTION sinh(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'sinh_v';
CREATE FUNCTION cosh(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'cosh_v';
CREATE FUNCTION tanh(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'tanh_v';
CREATE FUNCTION arcsinh(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'arcsinh_v';
CREATE FUNCTION arccosh(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'arccosh_v';
CREATE FUNCTION arctanh(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'arctanh_v';

--Rounding functions
CREATE FUNCTION rint(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'rint_v';
CREATE FUNCTION floor(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'floor_v';
CREATE FUNCTION ceil(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'ceil_v';
CREATE FUNCTION trunc(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'trunc_v';

-- Sums, products and differences

-- Exponential Functions
CREATE FUNCTION exp(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'exp_v';
CREATE FUNCTION expm1(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'expm1_v';
CREATE FUNCTION exp2(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'exp2_v';

CREATE FUNCTION log(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'log_v';
CREATE FUNCTION log10(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'log10_v';
CREATE FUNCTION log2(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'log2_v';
CREATE FUNCTION log1p(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'log1p_v';

CREATE FUNCTION clip(anyarray, anyelement, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'clip_c';

CREATE FUNCTION minimum(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'minimum_c';
CREATE FUNCTION maximum(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'maximum_c';

CREATE FUNCTION plus(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'plus_c';
CREATE FUNCTION add(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'plus_c';
CREATE FUNCTION multiply(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'multiply_c';
CREATE FUNCTION minus(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'minus_c';
CREATE FUNCTION divide(anyarray, anyelement) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'divide_c';

-- Binary functions
CREATE FUNCTION plus(anyarray, anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'plus';
CREATE FUNCTION minus(anyarray, anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'minus';
CREATE FUNCTION multiply(anyarray, anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'multiply';
CREATE FUNCTION divide(anyarray, anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'divide';

CREATE FUNCTION greater(anyarray, anyarray) RETURNS boolean[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'greater';
CREATE FUNCTION greater_equal(anyarray, anyarray) RETURNS boolean[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'greater_equal';


-- Vector Aggregate functions
CREATE FUNCTION asum(anyarray) RETURNS anyelement LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'asum';
CREATE FUNCTION amean(anyarray) RETURNS anyelement LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'amean';
CREATE FUNCTION avariance(anyarray) RETURNS anyelement  LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'avariance';
CREATE FUNCTION askew(anyarray) RETURNS anyelement LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'askew';
CREATE FUNCTION akurtosis(anyarray) RETURNS anyelement LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'akurtosis';

CREATE FUNCTION amin(anyarray) RETURNS anyelement LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'amin';
CREATE FUNCTION amax(anyarray) RETURNS anyelement LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'amax';


CREATE FUNCTION aall(anyarray) RETURNS boolean LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'aall';

-- Internal Aggregate Functions
CREATE FUNCTION internal_to_array(internal, anyarray) RETURNS anyarray LANGUAGE C IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'internal_to_array';
CREATE FUNCTION internal_to_array_dbl(internal) RETURNS double precision[] LANGUAGE C IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'internal_to_array';

CREATE FUNCTION sum_int(internal, anyarray) RETURNS internal LANGUAGE C IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'v_sum';
CREATE AGGREGATE sum(anyarray) ( SFUNC = sum_int, STYPE = internal, FINALFUNC=internal_to_array, finalfunc_extra );

CREATE FUNCTION avg_int(internal, anyarray) RETURNS internal LANGUAGE C IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'v_mean';
CREATE AGGREGATE avg(anyarray) ( SFUNC = avg_int, STYPE = internal, FINALFUNC=internal_to_array, finalfunc_extra );

CREATE FUNCTION skew_int(internal, anyarray) RETURNS internal LANGUAGE C IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'v_skew';
CREATE AGGREGATE skew(anyarray) ( SFUNC = skew_int, STYPE = internal, FINALFUNC=internal_to_array_dbl );


-- Other functions
CREATE FUNCTION sort(v real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'sort';

--Operators
CREATE OPERATOR + (LEFTARG = anyarray, RIGHTARG = anyelement, PROCEDURE = plus );
CREATE OPERATOR + (LEFTARG = anyarray, RIGHTARG = anyarray, PROCEDURE = plus );

CREATE OPERATOR - (LEFTARG = anyarray, RIGHTARG = anyelement, PROCEDURE = minus );
CREATE OPERATOR - (LEFTARG = anyarray, RIGHTARG = anyarray, PROCEDURE = minus );

CREATE OPERATOR * (LEFTARG = anyarray, RIGHTARG = anyelement, PROCEDURE = multiply );
CREATE OPERATOR * (LEFTARG = anyarray, RIGHTARG = anyarray, PROCEDURE = multiply );

CREATE OPERATOR / (LEFTARG = anyarray, RIGHTARG = anyelement, PROCEDURE = divide );
CREATE OPERATOR / (LEFTARG = anyarray, RIGHTARG = anyarray, PROCEDURE = divide );


-- Slice functions

CREATE FUNCTION slice(anyarray, integer[]) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'slice_idx';
CREATE FUNCTION slice(anyarray, boolean[]) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'slice_bit';

CREATE OPERATOR @ (LEFTARG = anyarray, RIGHTARG = integer[], PROCEDURE = slice);
CREATE OPERATOR @ (LEFTARG = anyarray, RIGHTARG = boolean[], PROCEDURE = slice);
