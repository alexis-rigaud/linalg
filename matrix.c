#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "matrix.h"
#include "vector.h"
#include "errors.h"
#include "util.h"
#include "linalg_obj.h"


struct matrix* matrix_new(int n_row, int n_col) {
    // TODO: Check that n_row and n_col are reasonable.
    struct matrix* new_matrix = malloc(sizeof(struct matrix));
    check_memory((void*)new_matrix);

    DATA(new_matrix) = malloc((sizeof(double)) * n_row * n_col);
    check_memory((void*) DATA(new_matrix));

    new_matrix->n_row = n_row;
    new_matrix->n_col = n_col;
    OWNS_MEMORY(new_matrix) = true;
    MEMORY_OWNER(new_matrix) = NULL;
    REF_COUNT(new_matrix) = 0;

    return new_matrix;
}

struct matrix* matrix_from_array(double* data, int n_row, int n_col) {
    struct matrix* M = matrix_new(n_row, n_col);
    for(int i = 0; i < n_row * n_col; i++) {
        DATA(M)[i] = data[i];
    }
    return M;
}

void matrix_free(struct matrix* M) {
    struct linalg_obj* mem_owner;
    if(OWNS_MEMORY(M)) {
        if(REF_COUNT(M) == 0) {
            free(DATA(M));
            free(M);
        } else {
            raise_non_zero_reference_free_error();
        }
    } else {
        if(REF_COUNT(M) == 0) {
            mem_owner = MEMORY_OWNER(M);
            REF_COUNT(mem_owner) -= 1;
            free(M);
        } else {
            raise_non_zero_reference_free_error();
        }
    }
}

struct vector* matrix_row_view(struct matrix* M, int row) {
    //TODO: Check that row is in bounds.
    double* row_p = DATA(M) + (row * M->n_col);
    struct vector* r = vector_new_view((struct linalg_obj*) M, row_p, M->n_col);
    return r;
}

struct vector* matrix_row_copy(struct matrix* M, int row) {
    //TODO: Check that row is in bounds.
    struct vector* r = vector_new(M->n_col);
    for(int i = 0; i < M->n_col; i++) {
        VECTOR_IDX_INTO(r, i) = MATRIX_IDX_INTO(M, row, i);
    }
    return r;
}

struct vector* matrix_column_copy(struct matrix* M, int col) {
    //TODO: Check that column is in bounds.
    struct vector* c = vector_new(M->n_row);
    for(int i = 0; i < M->n_row; i++) {
        VECTOR_IDX_INTO(c, i) = MATRIX_IDX_INTO(M, i, col);
    }
    return c;
}

void matrix_copy_vector_into_row(struct matrix* M, struct vector* v, int row) {
    //TODO: Check alignment.
    for(int i = 0; i < v->length; i++) {
        MATRIX_IDX_INTO(M, row, i) = VECTOR_IDX_INTO(v, i);
    }
}

void matrix_copy_vector_into_column(struct matrix* M, struct vector* v, int col) {
    //TODO: Check alignment.
    for(int i = 0; i < v->length; i++) {
        MATRIX_IDX_INTO(M, i, col) = VECTOR_IDX_INTO(v, i);
    }
}

struct matrix* matrix_zeros(int n_row, int n_col) {
    struct matrix* M = matrix_new(n_row, n_col);
    for(int i = 0; i < n_row * n_col; i++) {
        DATA(M)[i] = 0;
    }
    return M;
}

struct matrix* matrix_identity(int size) {
    struct matrix* M = matrix_new(size, size);
    for(int i = 0; i < size * size; i++) {
        if(MATRIX_ROW(M, i) == MATRIX_COL(M, i)) {
            DATA(M)[i] = 1;
        } else {
            DATA(M)[i] = 0;
        }
    }
    return M;
}

struct matrix* matrix_transpose(struct matrix* M) {
    struct matrix* Mt = matrix_new(M->n_col, M->n_row);
    for(int i = 0; i < M->n_row * M->n_col; i++) {
         DATA(Mt)[i] = DATA(M)[MATRIX_IDX(M, MATRIX_COL(Mt, i), MATRIX_ROW(Mt, i))];
    }
    return Mt;
}

struct matrix* matrix_multiply(struct matrix* Mleft, struct matrix* Mright) {
    //TODO: Check that dimensions are commensurate.
    struct matrix* Mprod = matrix_new(Mleft->n_row, Mright->n_col);
    double sum;
    for(int i = 0; i < Mprod->n_row; i++) {
        for(int j = 0; j < Mprod->n_col; j++) {
            sum = 0;
            for(int k = 0; k < Mleft->n_col; k++) {
                sum += MATRIX_IDX_INTO(Mleft, i, k) * MATRIX_IDX_INTO(Mright, k, j);
            }
            MATRIX_IDX_INTO(Mprod, i, j) = sum;
        }
    }
    return Mprod;
}

struct vector* matrix_vector_multiply(struct matrix* M, struct vector* v) {
    //TODO: Check that dimensions work out.
    struct vector* w = vector_new(v->length);
    double sum;
    for(int i = 0; i < M->n_row; i++) {
        sum = 0;
        for(int j = 0; j < M->n_col; j++) {
            sum += MATRIX_IDX_INTO(M, i, j) * VECTOR_IDX_INTO(v, j);
        }
        VECTOR_IDX_INTO(w, i) = sum;
    }
    return w;
}

bool matrix_equal(struct matrix* M1, struct matrix* M2, double tol) {
    if(M1->n_row != M2->n_row) {
        return false;
    }
    if(M1->n_col != M2->n_col) {
        return false;
    }
    for(int i = 0; i < M1->n_row; i++) {
        for(int j = 0; j < M1->n_col; j++) {
            if(fabs(MATRIX_IDX_INTO(M1, i, j) - MATRIX_IDX_INTO(M2, i, j)) > tol) {
                return false;
            }
        }
    }
    return true;
}

void matrix_print(struct matrix* M) {
    struct vector* current_row;
    printf("[\n");
    for(int i = 0; i < M->n_row; i++) {
        printf("  ");
        current_row = matrix_row_view(M, i);
        vector_print(current_row);
        vector_free(current_row);
    }
    printf("]\n");
}


struct qr_decomp* qr_decomp_new(struct matrix* M) {
    struct qr_decomp*  qr = malloc(sizeof(struct qr_decomp));
    return qr;
}

void qr_decomp_free(struct qr_decomp* qr) {
    matrix_free(qr->q);
    matrix_free(qr->r);
    free(qr);
}

struct qr_decomp* matrix_qr_decomposition(struct matrix* M) {
    struct qr_decomp* qr = qr_decomp_new(M);
    struct matrix* q = matrix_new(M->n_row, M->n_col);
    struct matrix* r = matrix_zeros(M->n_col, M->n_col);
    struct vector* current_column;
    struct vector* current_unit_vector;
    double current_dot_product;
    double norm;
    
    for(int i = 0; i < M->n_col; i++) {
        current_column = matrix_column_copy(M, i);
        for(int j = 0; j < i; j++) {
            // Do the projection math
            current_unit_vector = matrix_column_copy(q, j);
            current_dot_product = vector_dot_product(current_unit_vector, current_column);
            vector_scalar_multiply_into(current_unit_vector, current_dot_product);
            vector_subtract_into(current_column, current_unit_vector);
            vector_free(current_unit_vector);
            MATRIX_IDX_INTO(r, j, i) = current_dot_product;
        }
        norm = vector_norm(current_column);
        MATRIX_IDX_INTO(r, i, i) = norm;
        vector_normalize_into(current_column);
        matrix_copy_vector_into_column(q, current_column, i);
        vector_free(current_column);
    }

    qr->q = q; 
    qr->r = r;
    return qr;
}
