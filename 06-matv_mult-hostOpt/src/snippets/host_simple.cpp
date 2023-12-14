
//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//



#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>


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
#define NUM_CU 1
#include <CL/cl2.hpp>
#include <CL/cl_ext_xilinx.h>

#define DIM_SIZE 64
#define KERNEL_CL "matv_mult_dataflow"

using namespace std;

vector<unsigned char> read_binary_file(const std::string &filename)
{
    std::cout << "INFO: Reading " << filename << std::endl;

    std::ifstream file(filename, std::ios::binary);
    //Check if the file is open
    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open file: " << filename << std::endl;
        exit(1);
    }
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

int main(int argc, char** argv) {
    
    cl_int err;

    // read arguments 
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " device name kernel" <<  endl;
        return EXIT_FAILURE;
    }

    string binary_file = argv[2];
    cout<<binary_file<<endl;
    //***************************************************
    // STEP 0: Initialize data 
    //***************************************************
    
    // initialize matrix
    int i,j;
    // initialize matrices (vector of vectors)
    int matA[DIM_SIZE][DIM_SIZE];
    int matB[DIM_SIZE][DIM_SIZE];

    int vecB[DIM_SIZE];
    int res[DIM_SIZE][DIM_SIZE];
    // trasnpose matrix res
    int Tres[DIM_SIZE][DIM_SIZE];

    // initialize matrix and vectors 
    for(i=0;i<DIM_SIZE;i++){
        for(j=0;j<DIM_SIZE;j++){
            matA[i][j] = 1;
            matB[i][j] = j+1;
        }
        vecB[i] = 1;
    }

    // transpose matrix B
    int TmatB[DIM_SIZE][DIM_SIZE];
    for(i=0;i<DIM_SIZE;i++){
        for(j=0;j<DIM_SIZE;j++){
            TmatB[i][j] = matB[j][i];
        }
    }
    
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
    
    cl::Context context(device, nullptr, nullptr, nullptr, &err);
    cout << "CONTEXT ERROR: " << err << endl;

    //***************************************************
    // STEP 4: Create a command queue 
    //***************************************************
    // we create a command queue with the selected device and context using CommandQueue class 
    
    bool ooo = true;

    cl_ulong qp = CL_QUEUE_PROFILING_ENABLE;
    if (ooo)
        qp = qp | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;

    cl::CommandQueue q(context, device, qp, &err);
    
    cout << "COMMAND QUEUE ERROR: " << err << endl;

    //**************a*************************************
    // STEP 6: Create a program object for the context
    //***************************************************
    // read the kernel .xo file 

    cl::Kernel kernel;
    auto program_binary = read_binary_file(binary_file);
    cl::Program::Binaries bins{{program_binary.data(), program_binary.size()}};
    
    std::cout << "Trying to program device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    cl::Program program(context, {device}, bins, nullptr, &err);
    cout << "PROGRAM ERROR: " << err << endl;

    //***************************************************
    // STEP 6: Create the kernel object
    //***************************************************
    cl::Kernel krnls[NUM_CU];
    if (err != CL_SUCCESS) {
        std::cout << "Failed to program device with xclbin file!\n";
        cout << err << endl;
    } else {
        for (int i = 0; i < NUM_CU;i++){
            std::cout << "Device: program successful!\n";
            krnls[i] = cl::Kernel(program, KERNEL_CL, &err);
         // we break because we found a valid device
        }
    }
    //***************************************************
    // STEP 7: Create buffer for matrix A 
    //***************************************************
    cl::Buffer buffer_a(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, DIM_SIZE * DIM_SIZE * sizeof(int), (int*)matA, &err);    
    
    //***************************************************
    // STEP 8: Create buffers for each row of transposed matrix B and result
    //***************************************************
    cl::Buffer buffers_b[DIM_SIZE];
    cl::Buffer buffers_res[DIM_SIZE];
    
    for (int i = 0; i < DIM_SIZE; i++) {
        buffers_b[i] = cl::Buffer(context,  CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, DIM_SIZE * sizeof(int), &TmatB[i][0], &err);
        buffers_res[i] = cl::Buffer(context,  CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, DIM_SIZE * sizeof(int), &Tres[i][0], &err);
    }

    for (i = 0; i < DIM_SIZE; i=i+NUM_CU) {
        
        for (int j = 0; j < NUM_CU; j++) {

            // set kernel arguments
            krnls[j].setArg(0, buffers_res[i+j]);
            krnls[j].setArg(1, buffer_a);
            krnls[j].setArg(2, buffers_b[i+j]);
            
            // migrate memory
            q.enqueueMigrateMemObjects({buffer_a, buffers_b[i+j]}, 0 /* 0 means from host*/);
            //  run the kernel
            q.enqueueTask(krnls[j]);

        }
    }
    
    q.finish();

    for (i = 0; i < DIM_SIZE; i=i+NUM_CU) {
        for (int j = 0; j < NUM_CU; j++) {
            q.enqueueMigrateMemObjects({buffers_res[i+j]}, CL_MIGRATE_MEM_OBJECT_HOST);
        }
    }
    
    q.finish();



    // Transpose matrix Tres
    for (int i = 0; i < DIM_SIZE; i++) {
        for (int j = 0; j < DIM_SIZE; j++) {
            res[j][i] = Tres[i][j];
        }
    }

    int sum = 0;
    cout << "RESULT: " << endl;
    for ( i = 0; i < DIM_SIZE; i++){
        for (j = 0; j < DIM_SIZE; j++){
            cout << res[i][j]/64.0 << ", ";
        }
        cout << endl;
    }
    cout << endl;
    
    return 0;
}


// Types of datatransfer
// MigrateObject -> immeadiatly transfers the data from host to device
// writeBuffer -> transfers the data from host to device when kernel starts 
// i loop and the kernel latches the wrong number. Maybe wait for the kernel to start ?
// https://xilinx.github.io/Vitis_Accel_Examples/2021.1/html/data_transfer.html
// Tips and tricks: https://docs.xilinx.com/r/2021.2-English/ug1393-vitis-application-acceleration/Compute-Unit-Scheduling