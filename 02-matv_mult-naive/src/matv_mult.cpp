//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//


extern "C" {
    void matv_mult(int* c,
        const int* a,
        const int* b,
        const int dims){
        
        #pragma HLS interface m_axi port=a bundle=aximm1
        #pragma HLS interface m_axi port=b bundle=aximm2
        #pragma HLS interface m_axi port=c bundle=aximm1

        I_loop:
        for (int i = 0; i < dims; i++) {
            // c[i] = 0;
            *(c + (i)) = 0;
            J_loop:
            for (int j = 0; j < dims; j++) {
                // c[i] += a[i][j] * b[j];
                *(c + (i)) += *(a + (i * dims + j)) * *(b + (j));
            }
        }
    }
   
}