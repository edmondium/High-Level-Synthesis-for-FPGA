
#define BLOCK_DIM_SIZE 64   

void load_matA(int* a_local, const int* a) {
    // load matrix a into local RAM 
    I_load_a:
    for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
        // we use loop_tripcount only for analysis purpose
        // it does not translate to hardware 
        #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
        J_load_a:
        for (int j = 0; j < BLOCK_DIM_SIZE; j++) {
            #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE 
             *(a_local + (i * BLOCK_DIM_SIZE + j)) = *(a + (i * BLOCK_DIM_SIZE + j));
        }
    
    }
}

void load_vecB(int* b_local, const int* b) {
    // load vector b into local RAM
    I_load_b:
    for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
        #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
        b_local[i] = *(b + i);
    }
}

void mat_mult(int* c_local,  int* a_local,  int* b_local) {
    // compute matrix vector product
    int tmp;
    I_loop:
    for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
        #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
        tmp = 0;            
        J_loop:
        for (int j = 0; j < BLOCK_DIM_SIZE; j++) {
            #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
            tmp += a_local[i*BLOCK_DIM_SIZE + j] * b_local[j];
        }
        c_local[i] = tmp;
    }
}

void store_result(int* c, int* c_local) {
    // store vector c into global RAM
    I_store_c:
    for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
        #pragma HLS loop_tripcount max=BLOCK_DIM_SIZE
        *(c + i) = c_local[i];
    }
}


extern "C" {
    void matv_mult_dataflow(int* c,
        const int* a,
        const int* b){
        
        #pragma HLS interface m_axi port=a bundle=aximm1 max_read_burst_length = 16 max_write_burst_length = 16
        #pragma HLS interface m_axi port=b bundle=aximm2 max_read_burst_length = 16 max_write_burst_length = 1
        #pragma HLS interface m_axi port=c bundle=aximm1 max_read_burst_length = 16 max_write_burst_length = 16

        #pragma HLS dataflow
        // max_read_burst_length denotes the number of elements in the burst
        int a_local[BLOCK_DIM_SIZE][BLOCK_DIM_SIZE];  
        
        int b_local[BLOCK_DIM_SIZE];
        int c_local[BLOCK_DIM_SIZE];
        
        
        #pragma HLS array_partition variable=a_local type=complete
        #pragma HLS array_partition variable=b_local type=complete
        #pragma HLS array_partition variable=c_local type=complete
        
        // load matrix a into local RAM 
        load_matA(&a_local[0][0], a);


        
        // load vector b into local RAM
        load_vecB(b_local, b);


        // compute matrix vector product
        mat_mult(c_local, &a_local[0][0], b_local);
        
        
        // store result in global memory
        store_result(c, c_local);

    }
   
}