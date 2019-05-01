SELECT '{1,2}'::integer[] + '{3,4}'::integer[];

-- One operand
SELECT '{}'::integer[] + 3;
SELECT '{1,2,5}'::integer[] + 3;
SELECT '{{1,2},{3,4}}'::integer[] + 3;
SELECT '{1,2}'::integer[] + 3.5;

