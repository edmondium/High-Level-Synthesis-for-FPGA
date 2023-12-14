#ifndef MATRIX_VECTOR_MULT_DATAFLOW_H
#define MATRIX_VECTOR_MULT_DATAFLOW_H

#define BLOCK_DIM_SIZE 128

extern "C" {
    void matv_mult_dataflow(int* c,
        const int* a,
        const int* b);
}

#endif  // MATRIX_VECTOR_MULT_H