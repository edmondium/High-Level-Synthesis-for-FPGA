
//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//



#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <random>
#include <iomanip>


// XRT includes
#include "xrt/xrt_bo.h"
#include <experimental/xrt_xclbin.h>
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

// OpenCL API 
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL/cl2.hpp>
#include <CL/cl_ext_xilinx.h>

#define N 8
#define KERNEL_CL "cho_dec"

using namespace std;

vector<unsigned char> read_binary_file(const std::string &filename)
{
    std::cout << "INFO: Reading " << filename << std::endl;
    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> data;
    data.reserve(file_size);
    data.insert(data.begin(),
        std::istream_iterator<unsigned char>(file),
        std::istream_iterator<unsigned char>());

    return data;
}

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


int main(int argc, char** argv) {
    
    cl_int err;

    // read arguments 
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " device name kernel" << endl;
        return EXIT_FAILURE;
    }

    string binary_file = argv[2];
    cout<<binary_file<<endl;
    //***************************************************
    // STEP 0: Initialize data 
    //***************************************************
    
    int A[N][N];
    int L[N][N];
    int L1[N][N];

    generateLowerTriangularMatrix(L);
    mat_mult(A, L);



    //***************************************************
    // STEP 1: Get the platform 
    //***************************************************
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform;

    for(cl::Platform &p: platforms)
    {
        const string name = p.getInfo<CL_PLATFORM_NAME>();
        cout << "PLATFORM: " << name << endl;
        if(name == "Xilinx")
        {
            platform = p;
            break;
        }
    }

    if(platform == cl::Platform())
    {
        cout << "Xilinx platform not found!" << endl;
        exit(EXIT_FAILURE);
    }
    
    //***************************************************
    // STEP 2: Get the devices and select the desired device 
    //***************************************************

    vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
    
    cout<<"Number of devices found: " << devices.size() << endl;

    cl::Device device;
    for(cl::Device &iterDevice: devices){
        cout << "DEVICE: " << iterDevice.getInfo<CL_DEVICE_NAME>() << endl;
        if(iterDevice.getInfo<CL_DEVICE_NAME>() == argv[1])
            device = iterDevice;
    }
    
    cout << "SELECTED DEVICE: " << device.getInfo<CL_DEVICE_NAME>() << endl;
    
    //***************************************************
    // STEP 3: Create a context 
    //***************************************************
    // we create a context with the selected device using Context class 
    
    // TODO: Create context using cl::Context class

    //***************************************************
    // STEP 4: Create a command queue 
    //***************************************************
    // we create a command queue with the selected device and context using CommandQueue class 
    
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err);
    cout << "COMMAND QUEUE ERROR: " << err << endl;

    //***************************************************
    // STEP 5: Create device buffers
    //***************************************************

    // TODO: Create device buffers using cl::Buffer class
    // cl::Buffer buffer_a();    
    // cl::Buffer buffer_res();

    //***************************************************
    // STEP 6: Create a program object for the context
    //***************************************************
    // read the kernel .xo file 

    cl::Kernel kernel;
    auto program_binary = read_binary_file(binary_file);
    cl::Program::Binaries bins{{program_binary.data(), program_binary.size()}};
    
    std::cout << "Trying to program device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    cl::Program program(context, {device}, bins, nullptr, &err);
    //***************************************************
    // STEP 6: Create the kernel object
    //***************************************************
    
    if (err != CL_SUCCESS) {
        std::cout << "Failed to program device with xclbin file!\n";
        cout << err << endl;
    } else {
        std::cout << "Device: program successful!\n";
        kernel = cl::Kernel(program, KERNEL_CL, &err);
         // we break because we found a valid device
    }

    //***************************************************
    // STEP 7: Write host data to device buffers
    //***************************************************

    q.enqueueMigrateMemObjects({buffer_a}, 0 ); /* 0 means from host*/

    //***************************************************
    // STEP 8: Set the kernel arguments
    //***************************************************

    // TODO: Set kernel arguments

    //***************************************************
    // STEP 9: Enqueue the kernel for execution
    //***************************************************

    // TODO: Enqueue kernel
    
    //***************************************************
    // STEP 12: Read the output buffer back to the host
    //***************************************************
    
    // TODO: Read the output buffer back to the host

    q.finish();

    
    cout << "Checking results: " << endl;
    
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
