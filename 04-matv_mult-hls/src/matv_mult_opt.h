#ifndef MATRIX_VECTOR_MULT_H
#define MATRIX_VECTOR_MULT_H

#define BLOCK_DIM_SIZE 128

extern "C" {
    void matv_mult_opt(int* c,
        const int* a,
        const int* b);
}

#endif  // MATRIX_VECTOR_MULT_H