//#pragma OPENCL EXTENSION cl_khr_fp64 : enable


struct linalg_obj {
    bool owns_memory;
    struct linalg_obj* memory_owner;
    int ref_count;
    double* data;
};

#ifndef OWNS_MEMORY
#define OWNS_MEMORY(object) (((struct linalg_obj*) object)->owns_memory)
#endif

#ifndef MEMORY_OWNER
#define MEMORY_OWNER(object) (((struct linalg_obj*) object)->memory_owner)
#endif

#ifndef REF_COUNT
#define REF_COUNT(object) (((struct linalg_obj*) object)->ref_count)
#endif

#ifndef DATA
#define DATA(object) (((__global struct linalg_obj*) object)->data)
#endif

struct matrix {
    struct linalg_obj la_obj;
    int n_row;
    int n_col;
};

#ifndef _MATRIX_MACROS
#define _MATRIX_MACROS
#define MATRIX_ROW(M, i) ((i) / (M->n_row))
#define MATRIX_COL(M, i) ((i) % (M->n_row))
#define MATRIX_IDX(M, r, c) (((r) * (M->n_col)) + (c))
#define MATRIX_IDX_INTO(M, r, c) (DATA(M)[MATRIX_IDX(M, r, c)])
#endif


__kernel void matmult(__global struct matrix* Mprod, __global struct matrix* Mleft, __global struct matrix* Mright)
{
    int i = get_global_id(0); // for int i loop (to Mprod->n_row)
    int k = get_global_id(1); // for int k loop (to Mleft->n_row)
    int j = get_global_id(2); // for int j loop (to Mprod->n_col)

    MATRIX_IDX_INTO(Mprod, i, j) += MATRIX_IDX_INTO(Mleft, k, i) * MATRIX_IDX_INTO(Mright, k, j);
}
