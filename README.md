linalg: Linear Algebra and Regression in C
===========================================

> reboot  the Matthew Drury's linalg library, for computation on CPU, MCU and GPU target.

`linalg` is a library for linear algebra and regression implemented in C.  The code is optimized for readability and clarity instead of raw efficiency (though it tries not to ignore issues of efficiency completely).

Linear Algebra
--------------

`linalg` contains two datatypes in its core linear algebra engine, `vector` and `matrix`:

  - `vector` is a (one dimensional) vector of real numbers (C `double`s).  The underlying data is stored as a C array, and so occupies contiguous memory locations in the computer's memory.
  - `matrix` is a (two dimensional) matrix of real numbers (C `double`s).  The underlying data is stored in row-major order, so each row occupies contiguous memory locations in the computer's memory.

To complement these data types, `linalg` contains many functions for performing linear algebraic operations.  For example

  - `matrix_vector_multiply` computes the product vector of a matrix and vector.
  - `matrix_multiply` computes the product matrix of two matrices.
  - `matrix_multiply_MtN` computes the product of the transpose of one matrix with another.

Linear equations can be solved using `linsolve_qr`, which adopts a strategy of computing the QR matrix factorization of the left hand side.  To access the underlying matrix factorization, use `qr_decomp`.

Regression
----------

`linalg` also includes functions for regression.  Use `linreg_fit` to fit a linear regression given a design matrix `X` and a response vector `y`.

Tests
-----

The routines in `linalg` are extensively unit tested, which also serve as simple examples of use.

Copyright
-----

See files   
(c) Matthew Drury, 2017   
(c) Alexis Rigaud, 2024   

TODO
-----

[x] - add modern CMake support   
[x] - add OpenCL kernels support
[ ] - use size_t for loop counters
[ ] - use restrict pointer for data
[ ] - fix kernel compilation path
[ ] - implement kernels   
[ ] - implement SVD   

Build info
-----

`rm -fr build; mkdir build; cd build; cmake ..; make`

