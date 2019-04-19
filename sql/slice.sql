-- Integer arrays
SELECT ARRAY[1,2,3,4,5,6] @ '{}'::integer[];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[1];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[-1];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[1,1,1,-1,-1];

-- Boolean slices
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[true, true, false, false, false, true];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[false, false, false, false, false, false];

--Other Types
SELECT ARRAY['asd', 'fds', 'wef'] @ ARRAY[2, -2, 2];

-- out of bounds
SELECT '{}'::real[] @ ARRAY[1];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[10];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[0];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[-7];

--bit array errors
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[true, true, false, false, false];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[true, true, false, false, false, true, true];
