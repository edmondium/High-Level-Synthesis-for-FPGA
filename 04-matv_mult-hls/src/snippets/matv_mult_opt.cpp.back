#include "matv_mult_opt.h"
#define BLOCK_DIM_SIZE 128

extern "C" {
    void matv_mult_opt(int* c,
        const int* a,
        const int* b,
        const int dims){
        
        #pragma HLS interface m_axi port=a bundle=aximm1 max_read_burst_length = 16 max_write_burst_length = 16
        #pragma HLS interface m_axi port=b bundle=aximm2 max_read_burst_length = 16 max_write_burst_length = 1
        #pragma HLS interface m_axi port=c bundle=aximm1 max_read_burst_length = 16 max_write_burst_length = 16

        // max_read_burst_length denotes the number of elements in the burst
        int a_local[BLOCK_DIM_SIZE][BLOCK_DIM_SIZE];        
        int b_local[BLOCK_DIM_SIZE];
        int c_local[BLOCK_DIM_SIZE];

        // load matrix a into local RAM 
        I_load_a:
        for (int i = 0; i < dims; i++) {
            // we use loop_tripcount only for analysis purpose
            // it does not translate to hardware 
            #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
            J_load_a:
            for (int j = 0; j < dims; j++) {
                #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE 
                a_local[i][j] = *(a + (i * dims + j));
            }
        }

        // load vector b into local RAM
        I_load_b:
        for (int i = 0; i < dims; i++) {
            #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
            b_local[i] = *(b + i);
        }

        // compute matrix vector product
        int tmp;
        I_loop:
        for (int i = 0; i < dims; i++) {
            #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
            tmp = 0;            
            J_loop:
            for (int j = 0; j < dims; j++) {
                #pragma HLS loop_tripcount max=128
                tmp += a_local[i][j] * b_local[j];
            }
            c_local[i] = tmp;
        }

        // store result in global memory
        I_store_c:
        for (int i = 0; i < dims; i++) {
            #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
            *(c + i) = c_local[i];
        }

    }
   
}