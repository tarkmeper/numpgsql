-- Basic sorting
SELECT sort('{}'::int[]);
SELECT sort('{1}'::int[]);
SELECT sort('{5,1,4,6}'::int[]);

--Support Types
SELECT sort('{5,1,4,6}'::smallint[]);
SELECT sort('{5,1,4,6}'::bigint[]);
SELECT sort('{5,1,4,6}'::real[]);
SELECT sort('{5,1,4,6}'::double precision[]);

-- Unsupported type
SELECT SORT(ARRAY[true,true,false]);

-- Check reverse
SELECT reverse('{}'::int[]);
SELECT reverse('{1}'::int[]);
SELECT reverse('{5,1,4,6}'::int[]);

-- Check other types
SELECT reverse(ARRAY[true,true,false]);
SELECT reverse(ARRAY['cat', 'dog', 'rat']);

--multi dimesnion 
SELECT reverse('{{5,1},{4,6}}'::int[]);
