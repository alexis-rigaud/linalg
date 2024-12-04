/* kernels.c
  (c) A. Rigaud, 2024
*/

// TODO : switch macos / others
#include <OpenCL/opencl.h>


#include "matrix.h"

void kernel_test(void);
void matrix_multiply_ocl(struct matrix* Mprod, struct matrix* Mleft, struct matrix* Mright);

struct opencl_s {
    cl_kernel        kernel;
    cl_context       context;
    cl_command_queue queue;
    cl_program       program;
    cl_platform_id   platform;
    cl_device_id     device;
};

#define OCL_MAX_DIM 4
struct params {
    cl_mem m_out;
    double* m_out_ptr;
    cl_mem m_left;
    cl_mem m_right;
    size_t m_out_size;
    size_t m_left_size;
    size_t m_right_size;
    size_t m_dim;
    size_t m_work[OCL_MAX_DIM];
};
