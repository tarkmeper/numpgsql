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

-- Binary functions
CREATE FUNCTION plus(l real[], r real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'add';
CREATE FUNCTION add(l real[], r real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'add';
CREATE FUNCTION multiply(l real[], r real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'multiplies';
CREATE FUNCTION minus(l real[], r real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'minus';

CREATE FUNCTION multiply(l real[], r boolean[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'multiplies_ab';


-- Vector Aggregate functions
CREATE FUNCTION avg_arr(l real[]) RETURNS real LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'mean';
CREATE FUNCTION mean(l real[]) RETURNS real LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'mean';

CREATE FUNCTION stdev_arr(l real[]) RETURNS real LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'stdev';
CREATE FUNCTION std(l real[]) RETURNS real LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'stdev';


-- Internal Aggregate Functions
CREATE AGGREGATE sum(real[]) ( SFUNC = plus, STYPE = real[], INITCOND = '{}' );


-- Other functions
CREATE FUNCTION sort(v real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'sort';

--Operators
CREATE OPERATOR + (LEFTARG = anyarray, RIGHTARG = anyelement, PROCEDURE = plus );
CREATE OPERATOR + (LEFTARG = real[], RIGHTARG = real[], PROCEDURE = plus );

CREATE OPERATOR - (LEFTARG = real[], RIGHTARG = real[], PROCEDURE = minus );

CREATE OPERATOR * (LEFTARG = anyarray, RIGHTARG = anyelement, PROCEDURE = multiply );
CREATE OPERATOR * (LEFTARG = real[], RIGHTARG = real[], PROCEDURE = multiply );

