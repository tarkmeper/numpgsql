-- This program is open source, licensed under the MIT License.
-- For license terms, see the LICENSE file.
--
-- Copyright (C) 2019: Mark Teper

-- Other functions

CREATE FUNCTION random_seed(integer) RETURNS void LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'set_seed';
CREATE FUNCTION random_state() RETURNS text LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'get_rgen_state';
CREATE FUNCTION random_state(text) RETURNS void LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'set_rgen_state';

CREATE FUNCTION random_rand(VARIADIC integer[]) RETURNS double precision[] LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'gen_rand';
CREATE FUNCTION random_randn(VARIADIC integer[]) RETURNS double precision[] LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'gen_randn';
CREATE FUNCTION random_randint(low bigint, high bigint, integer[]) RETURNS bigint[] LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'gen_randint';

CREATE FUNCTION random_permute(anyarray) RETURNS anyarray LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'permute';
CREATE FUNCTION random_choice(anyarray, shape int[], replace boolean) RETURNS anyarray LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'choice';


CREATE FUNCTION random_beta(alpha double precision, beta double precision, shape int[]) RETURNS double precision[] LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'gen_beta';
CREATE FUNCTION random_binomial(t int, p double precision, shape int[]) RETURNS integer[] LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'gen_binomial';

