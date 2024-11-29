#include "vector.h"
#include "matrix.h"

__kernel void qr_kernel(__global struct matrix* r, __global struct matrix* q, __global struct vector* current_column)
{
    int i = get_global_id(0); // for int i loop
    int j = get_global_id(1); // for int j loop

    struct vector current_unit_vector = matrix_column_copy_s(q, j);
    double current_dot_product = vector_dot_product(&current_unit_vector, current_column);
    vector_scalar_multiply_into(&current_unit_vector, &current_unit_vector, current_dot_product);
    vector_subtract_into(current_column, current_column, &current_unit_vector);
    MATRIX_IDX_INTO(r, j, i) = current_dot_product;
}
