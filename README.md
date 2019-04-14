# numpgsql
Array & Numerical computing library based on the popular Python Numpy library for Posgres PG/SQL Array types.  Allow for performing scientific and engineering calculations within a Postgres Database.

## Installation

To install clone or download and run:

    
    make && sudo make install
    

## TODO
Nearly everything.  Major items not yet covered:
* Broadcasting functions
* Automatic type conversion 
* API scope
* Documentation
* Regression Tests
* Automated-build process
* Support for "axis" aggregation functions
* Integration with numeric library (Eigen/Atlas)

## Documentation
Most functions support all numeric types (smallint, integer, bigint, real, double). See the file numpgsql--*.sql for the full list of functions.

Currently supported functions:

* Fundamental math functions (plus, minus, multiply, divide, pow, exp, sqrt, abs, mod, log, etc.). Operator overloading for arithmetic.
    ~~~~
    SELECT cos('{5, 1, 6, 4}'::real[]), '{1,3}'::integer[] + 4;
    {0.283662,0.540302,0.96017,-0.653644}, {5, 7}
    ~~~~  

* Basic sorting (sort)
    ~~~~
    SELECT sort('{5, 1, 6, 4}'::integer[])
    { 1, 4, 5, 6 }
    ~~~~  

* Statistical functions across a vector  (amean, astd, askew, amax, amin, akurtosis).  Framework can easily be exstended to support any aggregation supported within C++ Boost accumulator libraries.
    ~~~~
    SELECT amean('{5, 1, 6, 4}'::integer[])
    4
    ~~~~  
