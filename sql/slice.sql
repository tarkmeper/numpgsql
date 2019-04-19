-- Integer arrays
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[1];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[-1];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[1,1,1,-1,-1];

--Other Types
SELECT ARRAY['asd', 'fds', 'wef'] @ ARRAY[2, -2, 2];

-- out of bounds
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[10];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[0];
SELECT ARRAY[1,2,3,4,5,6] @ ARRAY[-7];

