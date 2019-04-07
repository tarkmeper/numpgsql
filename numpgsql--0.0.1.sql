-- This program is open source, licensed under the PostgreSQL License.
-- For license terms, see the LICENSE file.
--
-- Copyright (C) 2019: Mark Teper

\echo Use "CREATE EXTENSION numpgsql" to load this file. \quit

SET client_encoding = 'UTF8';

-- creation functions

-- unary functionss
CREATE FUNCTION negate(v real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'negate';

CREATE FUNCTION clip(v real[], l real, u real) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'clip_c';

CREATE FUNCTION minimum(v real[], c real) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'minimum_c';
CREATE FUNCTION maximum(v real[], c real) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'maximum_c';

CREATE FUNCTION plus(v real[], c real) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'plus_c';
CREATE FUNCTION multiply(v real[], c real) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'multiply_c';


CREATE FUNCTION greater(v real[], c real) RETURNS boolean[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'greater_c';

-- Binary functions
CREATE FUNCTION plus(l real[], r real[]) RETURNS real[] LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'add';
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
