//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//

#include "vadd.h"
#include <iostream>

using namespace std;

int main() {
    int a[SIZE];
    int b[SIZE];

    // initialize vectors
    for (int i = 0; i < SIZE; i++) {
        a[i] = i*2;
        b[i] = i;
    }

    // perform vadd on CPU
    int c_sw[SIZE];
    for (int i = 0; i < SIZE; i++) {
        c_sw[i] = a[i] + b[i];
    }

    // perform vadd on on FPGA
    int c_hw[SIZE];
    vadd(c_hw, a, b);

    // compare results
    for (int i = 0; i < SIZE; i++) {
        if (c_sw[i] != c_hw[i]) {
            cout << "Error: c_sw[" << i << "] = " << c_sw[i] << ", c_hw[" << i << "] = " << c_hw[i] << endl;
            return 1;
        }
    }
}