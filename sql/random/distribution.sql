SELECT random_seed(1);

SELECT random_beta(2,3, '{2,3}'::integer[]);
SELECT random_beta(-1,3, '{2,3}'::integer[]);
SELECT random_beta(2,-3, '{2,3}'::integer[]);

SELECT random_binomial(2,0.5, '{2,3}'::integer[]);
SELECT random_binomial(-2,0.5, '{2,3}'::integer[]);
SELECT random_binomial(2,3, '{2,3}'::integer[]);
SELECT random_binomial(2,-3, '{2,3}'::integer[]);



