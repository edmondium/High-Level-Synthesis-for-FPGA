//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//

//#include "cho_dec.h"
#include <stdint.h>
#include <cmath>

#define N 8


extern "C" {
    void cho_dec(int* L, 
                 const int* A){   
        

        // TODO: Enable burst read and write 
        // TODO: Enable separate bundles for L and A
        

        // TODO: Add pragmas for array partitioning
        int dataA[N][N];
        int dataL[N][N] = {{0}};
        int i,j,k;

        LoadA:
        for (int i = 0; i < N; ++i) {
            #pragma HLS loop_tripcount max=N
            for (int j = 0; j <= i; ++j) {
                #pragma HLS loop_tripcount max=N
                dataA[i][j] = A[i * N + j];
            }
        }

        int tmp0 = sqrt(dataA[0][0]);

        // Compute first column 
        dataL[0][0] = tmp0;
        
        Column_first:
        for ( i = 1; i < N; ++i) {
            #pragma HLS loop_tripcount max=N-1

            dataL[i][0] = dataA[i][0] / tmp0;
        }

        //compute other columns
        int tmp1 = 0;
        int tmp2 = 0;

        Column_other:
        for ( j = 1; j < N; ++j){
            #pragma HLS loop_tripcount max=N-1
            tmp1 = 0;
            // Compute diagonal elements
            Diag:
            for(int k = 0; k < j; k++){
                #pragma HLS loop_tripcount max=N-1
                tmp1 += dataL[j][k]*dataL[j][k];
            }
            dataL[j][j] = sqrt(dataA[j][j] - tmp1);

            // Compute other elements
            if (j < N-1){
                Rows:
                for( i = j+1; i < N; i++){
                    #pragma HLS loop_tripcount max=N-2
                    tmp2=0;
                
                    Vec_mul:
                    for( k = 0; k < j; k++){
                        #pragma HLS loop_tripcount max=N-2
                        tmp2 += dataL[i][k]*dataL[j][k];
                    }
                    dataL[i][j] = (dataA[i][j] - tmp2)/dataL[j][j];
                }
            }
        }

        // Store L
        StoreL:
        for (int i = 0; i < N; ++i) {
            #pragma HLS loop_tripcount max=N
            for (int j = 0; j < N; ++j) {
                #pragma HLS loop_tripcount max=N
                L[i * N + j] = dataL[i][j];
            }
        }  
    }
}
