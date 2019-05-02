SELECT random_seed(1);

SELECT random_randint(1,5,'{2}'::integer[]);

SELECT random_randint(1,5,'{2,2}'::integer[]);

-- some error-ish conditions
SELECT random_randint(5,1,'{2,2}'::integer[]);
SELECT random_randint(1,1,'{2,2}'::integer[]);
