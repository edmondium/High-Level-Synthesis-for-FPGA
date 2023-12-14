//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//
#include "vadd.h"

extern "C" {
    void vadd(int* c,
        const int* a,
        const int* b)
    {

        #pragma HLS interface m_axi port=a bundle=aximm1
        #pragma HLS interface m_axi port=b bundle=aximm2
        #pragma HLS interface m_axi port=c bundle=aximm1

        int i;
        int arrayA[SIZE];
        int arrayB[SIZE];
        int arrayC[SIZE];

        readA:
            for( i = 0; i < SIZE; i++)
                arrayA[i] = a[i];

        readB:
            for( i = 0; i < SIZE; i++)
                arrayB[i] = b[i];
        
        vadd: 
            for (i = 0; i < SIZE; i++)
                arrayC[i] = arrayA[i] + arrayB[i];

        writeC:
            for(int i = 0; i < SIZE; i++)
                c[i] = arrayC[i];
        
    }
}