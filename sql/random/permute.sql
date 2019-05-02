SELECT random_seed(1);

SELECT random_permute('{}'::integer[]);
SELECT random_permute('{1,2,3,4,5,6}'::integer[]);
SELECT random_permute('{{1,2,3},{4,5,6}}'::integer[]);

-- Choice without replacement
SELECT random_choice('{}'::integer[], '{3}'::integer[], false);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{3}'::integer[], false);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{2,2}'::integer[], false);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{7}'::integer[], false);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{3,2}'::integer[], false);


-- Choice with replacement
SELECT random_choice('{}'::integer[], '{3}'::integer[], true);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{3}'::integer[], true);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{2,2}'::integer[], true);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{7}'::integer[], true);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{3,2}'::integer[], true);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{3,2,4,2,1,2,3}'::integer[], true);
SELECT random_choice('{1,2,3,4,5}'::integer[], '{{3,2,1,4},{2,1,2,3}}'::integer[], true);
