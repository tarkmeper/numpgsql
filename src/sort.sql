-- This program is open source, licensed under the MIT License.
-- For license terms, see the LICENSE file.
--
-- Copyright (C) 2019: Mark Teper

-- Other functions
CREATE FUNCTION sort(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'sort';
CREATE FUNCTION reverse(anyarray) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'reverse';
