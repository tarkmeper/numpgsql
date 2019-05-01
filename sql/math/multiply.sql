SELECT multiply('{1,2}'::integer[], '{3,4}'::integer[]);

--Single value
SELECT '{{1,2},{3,4}}'::integer[] * 3;
