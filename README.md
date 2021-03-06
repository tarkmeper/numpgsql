# numpgsql
[![Build Status][travis-badge]][travis-link]
[![codecov](https://codecov.io/gh/tarkmeper/numpgsql/branch/master/graph/badge.svg)](https://codecov.io/gh/tarkmeper/numpgsql)
[![MIT License][license-badge]](LICENSE.md)


# numpgsql
Array & Numerical computing library based on the popular Python Numpy library for Posgres PG/SQL Array types.  Allow for performing scientific and engineering calculations within a Postgres Database.

The objective of this extension is to make it possible to run analytics and regressions on the postgres server without pulling what can be large volumes of data to a local clients. 

## Requirements

The library makes use of Boost C++ accumulators in order to do many of the statistical functions. On Debian/Ubuntu systems the boost libraries can be installed using.
    
    sudo apt-get install libboost-all-dev
    
The libary also makes use of templates and standard libraries.  Using old versions of gcc may be necessary to enable C++11x.

## Installation

To install clone or download and run:
    
    make && sudo make install

## Features:
  * Numeric operations
  * Advanced array slicing     
  * Array operations
  * Random distribution generation
  * Mathematical operations
  * Logical operations

## TODO
Many things.  Major items not yet covered:
* Broadcasting functions
* Type conversion 
* Documentation
* Support for "axis" aggregation functions
* Integration with numeric library (Eigen/Atlas)

## Examples

* Basic math functions:
    ~~~~
    SELECT cos('{5, 1, 6, 4}'::real[]), '{1,3}'::integer[] + 4;
    {0.283662,0.540302,0.96017,-0.653644}, {5, 7}
    ~~~~  

* Random Number Generation:
    ~~~~
    SELECT random_randn(2,2);
    {{1.58275229197514,-1.04514681044202},{0.25759354129149,-1.959394569747}}
    ~~~~  

* Slicing 
    ~~~~
    SELECT ARRAY[10,20,30,40,50,60] @ ARRAY[2,2,4,6];
    { 20, 20, 40, 60 }

    SELECT ARRAY[10,20,30] @ ARRAY[false, true, true];
    { 20, 30 }
    ~~~~

* Sorting:
    ~~~~
    SELECT sort('{5, 1, 6, 4}'::integer[])
    { 1, 4, 5, 6 }
    ~~~~  

* Statistical functions across a vector:
    ~~~~
    SELECT amean('{5, 1, 6, 4}'::integer[])
    4
    ~~~~  

* Statistical aggregate function:
    ~~~~
    SELECT sum(x.a) FROM ( SELECT '{3,5}' as a UNION ALL SELECT '{1,20}'::real[] UNION ALL SELECT '{9,10}'::real[]) x;
    { 13, 35 }
    ~~~~

## Documentation

Most functions support all numeric types (smallint, integer, bigint, real, double). See the file numpgsql--*.sql for the full list of functions.
  * Logic:
      * *aall(anyarray)*: Determine if all the elements in the array evaluate to True (non-zero for numeric types). _Todo add support for other non-numeric types.
  * Math:
      * *sin(anyarray)* - Generate the sin value for every element in the array. Result shape matches input shape.
  * Random Number Generation:
      * *random_seed(int)* - Set the random seed for the generator.
      * *random_state()* - Get the current state of the random number generator
      * *random_state(text)* - Set the current state of the random number generator
      * *random_rand(VARIADIC int)* - Create an n dimensional [0,1] uniform random number array based on the dimensions specified as input parameters
      * *random_randn(VARIADIC int)* - Create an n dimensional 0 mean, 1 std normally distributed random number array based on the dimensions specified as input parameters
      * *random_randint(low bigint, high bigint, shape int[])* - Create an n dimensional uniformally distributed random number array based on the dimensions specified in shape.  Low and and high bounds specified by input parameters.
      * *random_permute(array)* - Returns randomly permuted version of input array
      * *random_choice(anyarray, shape int[], replace boolean)* - Return randomly selected elements from input array based on shape, with or witout repalcement
      * *random_beta(alpha double, beta double, shape int[])* - Return a set of numbers from the beta distribution to match shape.
      * *random_binomial(t int, p double, shape int[])* - Return a set of numbers drawn from the binomial distribution.
  * Slicing: The "@" operator can be used instead of slice function.
      * *slice(anyarray, int[]):* - Create new array based on elements identified in second param.  Supports negative indexing from end.
      * *slice(anyarray, boolean[]):* - Create new array based on setting in boolean array.  Boolean array must have same dimensions and size as input array.
  * Sorting:
      * *sort(anyarray)* - Sort elements of an array in numeric order.  _Todo add support for non-numeric types_
      * *reverse(anyarray)* - Reverse order of elements in array.  Supports all array types


[1]: https://codecov.io/
[travis-badge]:    https://travis-ci.org/tarkmeper/numpgsql.svg?branch=master
[travis-link]:     https://travis-ci.org/tarkmeper/numpgsql
[license-badge]:   https://img.shields.io/badge/license-MIT-007EC7.svg
