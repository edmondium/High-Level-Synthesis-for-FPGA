//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>

const int N = 4;

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
void choleskyDecomposition(int* L, int* A)
{   
    int dataA[N][N];
    int dataL[N][N] = {{0}};
    int i,j,k;

    LoadA:
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            dataA[i][j] = A[i * N + j];
        }
    }

    int tmp0 = sqrt(dataA[0][0]);


    // Compute first column 
    dataL[0][0] = tmp0;
    
    Column_first:
    for ( i = 1; i < N; ++i) {
        dataL[i][0] = dataA[i][0] / tmp0;
    }


    //compute other columns
    int tmp1 = 0;
    int tmp2 = 0;

    Column_other:
    for ( j = 1; j < N; ++j){
        tmp1 = 0;

        // Compute diagonal elements
        Diag:
        for(int k = 0; k < j; k++){
            tmp1 += dataL[j][k]*dataL[j][k];
        }
        dataL[j][j] = sqrt(dataA[j][j] - tmp1);

        // Compute other elements
        if (j < N-1){
            Rows:
            for( i = j+1; i < N; i++){
                tmp2=0;
            
                Vec_mul:
                for( k = 0; k < j; k++){
                    tmp2 += dataL[i][k]*dataL[j][k];
                }
                dataL[i][j] = (dataA[i][j] - tmp2)/dataL[j][j];
            }
        }
    }

    // Store L
    StoreL:
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            L[i * N + j] = dataL[i][j];
        }
    }  
}

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



int main()
{
    int A[N][N] = {{0}};
    int L[N][N] = {{0}};
    int L1[N][N] = {{0}};

    // generate lower triangular matrix (LTM)
    generateLowerTriangularMatrix(L);

    // generate matrix A for LTM 
    mat_mult(A, L);
    
    // obtain LTM for matrix A
    //choleskyDecomposition(L1, A);
    choleskyDecomposition((int*)L1, (int*)A);

    // Compare L1 and L
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (L1[i][j] != L[i][j]) {
                cout << "Reference LTM: " << endl;
                printMatrix(L);
                cout << "Output LTM: " << endl;
                printMatrix(L1);
                cout << "ERROR" << endl;
                return 1;
            }
        }
    }
    cout << "SUCCESS" << endl;
    cout << "INPUT MATRIX: " << endl;
    printMatrix(A);
    cout << "OUTPUT LTM: " << endl;
    printMatrix(L1);
    return 0;
}