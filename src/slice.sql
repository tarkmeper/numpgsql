-- This program is open source, licensed under the MIT License.
-- For license terms, see the LICENSE file.
--
-- Copyright (C) 2019: Mark Teper

CREATE FUNCTION slice(anyarray, integer[]) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'slice_idx';
CREATE FUNCTION slice(anyarray, boolean[]) RETURNS anyarray LANGUAGE C STRICT IMMUTABLE LEAKPROOF COST 100 PARALLEL SAFE AS '$libdir/numpgsql', 'slice_bit';

CREATE OPERATOR @ (LEFTARG = anyarray, RIGHTARG = integer[], PROCEDURE = slice);
CREATE OPERATOR @ (LEFTARG = anyarray, RIGHTARG = boolean[], PROCEDURE = slice);
