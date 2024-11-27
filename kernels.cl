__kernel void mandelbrot_kernel(__global int *output, float center_re, float center_im, float scale, int max_iterations, int width, int height) {
    int x = get_global_id(0);
    int y = get_global_id(1);
    float c_re = center_re + (x - width / 2) * scale;
    float c_im = center_im + (y - height / 2) * scale;
    float z_re = 0;
    float z_im = 0;
    float z2_re = 0;
    float z2_im = 0;
    int iterations = 0;
    while ((z2_re + z2_im < 4) && (iterations++ < max_iterations)) {
        z_im = 2 * z_re * z_im + c_im;
        z_re = z2_re - z2_im + c_re;
        z2_re = z_re * z_re;
        z2_im = z_im * z_im;
    }
    float brightness = 24 ;
    float color = 255 * iterations / max_iterations;
    float srgb_color = color <= 0.0031308 ? color * 12.92 : 1.055 * pow(color, 1.0 / 2.4) - 0.055;
    output[x + y * width] = brightness * srgb_color;
}

#if 0

#include "vector.h"
#include "matrix.h"

__kernel void qr_kernel(__global struct matrix* r, __global struct matrix* q, __global struct vector* current_column) {
    int i = get_global_id(0); // for int i loop
    int j = get_global_id(1); // for int j loop

    struct vector* current_unit_vector = matrix_column_copy(q, j);
    double current_dot_product = vector_dot_product(current_unit_vector, current_column);
    vector_scalar_multiply_into(current_unit_vector,
                                current_unit_vector, current_dot_product);
    vector_subtract_into(current_column,
                            current_column, current_unit_vector);
    vector_free(current_unit_vector);
    MATRIX_IDX_INTO(r, j, i) = current_dot_product;
}

#endif
