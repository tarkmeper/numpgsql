SELECT random_seed(1);
SELECT random_state(random_state());

SELECT random_rand(4);
SELECT random_rand(4);

SELECT random_seed(1);
SELECT random_rand(4);


DO $$
DECLARE
   val text;
   r  text;
   r1a text;
   r2 text;
BEGIN
SELECT random_state() INTO val;
SELECT random_rand(4) INTO r;
RAISE NOTICE '%', r;

SELECT random_rand(4) INTO r1a;
RAISE NOTICE '%', r1a;


PERFORM random_state(val);
SELECT random_rand(4) INTO r2;
RAISE NOTICE '%', r2;

END $$;
