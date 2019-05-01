-- This program is open source, licensed under the MIT License.
-- For license terms, see the LICENSE file.
--
-- Copyright (C) 2019: Mark Teper

-- Other functions

CREATE FUNCTION random_seed(integer) RETURNS void LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'set_seed';
CREATE FUNCTION random_state() RETURNS text LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'get_rgen_state';
CREATE FUNCTION random_state(text) RETURNS void LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'set_rgen_state';

CREATE FUNCTION random_rand(VARIADIC integer[]) RETURNS double precision[] LANGUAGE C STRICT COST 100 AS '$libdir/numpgsql', 'gen_rand';
