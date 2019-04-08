# numpgsql
Array & Numerical computing library based on the popular Python Numpy library for Posgres PG/SQL Array types.  Allow for performing scientific and engineering calculations within a Postgres Database.

## Installation

To install clone or download and run:
    ~~~~
    make && sudo make install
    ~~~

## TODO
Nearly everything.  Major items not yet covered:
* Broadcasting functions
* Automatic type conversion 
* API scope
* Documentation
* Regression Tests
* Automated-build process
* Support for "axis" aggregation functions

## Documentation
Most functions support all numeric types (smallint, integer, longint, real, double). See the file numpgsql--*.sql for the full list of functions.

Currently supported functions:

* Fundamental math functions (plus, minus, multiply, divide, pow, exp, sqrt, abs, mod, log, etc.)
    ~~~~
    SELECT plus('{5, 1, 6, 4}'::integer[], 4), plus('{1,3}'::integer[], '{4,5}::integer[]);
    { 9, 5, 10, 8 }, { 5, 8 }
    ~~~~  
* Basic sorting (sort)
    ~~~~
    SELECT sort('{5, 1, 6, 4}'::integer[])
    { 1, 4, 5, 6 }
    ~~~~  
* Aggregation (mean, std)    
    ~~~~
    SELECT sort('{5, 1, 6, 4}'::integer[])
    { 1, 4, 5, 6 }
    ~~~~  
