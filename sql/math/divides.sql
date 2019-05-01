SELECT divide('{3,4}'::integer[], '{1,2}'::integer[]);

--Div by 0
SELECT divide('{3,4}'::integer[], '{0,0}'::integer[]);

--Single value
SELECT '{{6,9},{3,4}}'::integer[] / 3;
SELECT divide('{3,4}'::integer[], 0);
