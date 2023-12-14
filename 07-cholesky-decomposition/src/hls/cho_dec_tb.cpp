//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//

#include "../cho_dec.h"
#include <iostream>

#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>


using namespace std;
void generateLowerTriangularMatrix(int L[][N])
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j++) {
            if (i >= j) {
                L[i][j] = dist(gen);
            } else {
                L[i][j] = 0;
            }
        }
    }
}

// LINK: https://algowiki-project.org/en/Cholesky_decomposition
void mat_mult(int A[][N], int L[][N]){
    // A = L * L^T
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += L[i][k] * L[j][k]; 
            }
            A[i][j] = sum;
        }
    }
}
void printMatrix(int matrix[][N])
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}



int main(){
    int A[N][N] = {{0}};
    int L[N][N] = {{0}};
    int L_hw[N][N] = {{0}};

    // generate lower triangular matrix (LTM)
    generateLowerTriangularMatrix(L);
    printMatrix(L);
    // generate matrix A for LTM 
    mat_mult(A, L);
    
    // obtain LTM for matrix A from hardware simulation
    cho_dec((int*)L_hw, (int*)A);

    // Compare L1 and L
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (L_hw[i][j] != L[i][j]) {
                cout << "Reference LTM: " << endl;
                printMatrix(L);
                cout << "Output LTM: " << endl;
                printMatrix(L_hw);
                cout << "ERROR" << endl;
                return 1;
            }
        }
    }
    cout << "SUCCESS" << endl;
    cout << "INPUT MATRIX: " << endl;
    printMatrix(A);
    cout << "OUTPUT LTM: " << endl;
    printMatrix(L_hw);
    return 0;
}