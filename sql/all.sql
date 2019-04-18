select aall('{}'::integer[]);
select aall('{1}'::integer[]);
select aall('{0}'::integer[]);
select aall('{1,3,5,1}'::integer[]);
select aall('{1,3,0,1}'::integer[]);
select aall('{{1,3},{0,1}}'::integer[]);

select aall('{true}'::boolean[]);
select aall('{false}'::boolean[]);

