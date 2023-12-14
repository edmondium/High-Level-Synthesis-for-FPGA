
//
//  Created by Ratko Pilipovic on 22/11/2023.
//  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
//



#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <stdlib.h>

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

#define DATA_SIZE 1024
#define KERNEL_CL "vadd"


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
    else
    {
        std::cout << "INFO: File opened successfully" << std::endl;
    }
    
    file.unsetf(std::ios::skipws);


    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    cout << "INFO: File size: " << file_size << std::endl;

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
        cout << "Usage: " << argv[0] << " device name kernel" << endl;
        return EXIT_FAILURE;
    }

    string binary_file = argv[2];
    cout<<binary_file<<endl;
    //***************************************************
    // STEP 0: Initialize data 
    //***************************************************
    
    int a[DATA_SIZE];
    int b[DATA_SIZE];
    int c[DATA_SIZE];

    for (int i = 0; i < DATA_SIZE; i++) {
        a[i] = 1;
        b[i] = 1;
        c[i] = 0;
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
    
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err);
    cout << "COMMAND QUEUE ERROR: " << err << endl;

    //***************************************************
    // STEP 5: Create device buffers
    //***************************************************

    cl::Buffer buffer_a(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, DATA_SIZE * sizeof(int), a, &err);    

    cl::Buffer buffer_b(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, DATA_SIZE * sizeof(int), b, &err);    

    cl::Buffer buffer_res(context,  CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, DATA_SIZE * sizeof(int), c, &err);
    
    //***************************************************
    // STEP 6: Create a program object for the context
    //***************************************************
    // read the kernel .xo file 

    cl::Kernel kernel;
    auto program_binary = read_binary_file(binary_file);

    cout << "PROGRAM BINARY SIZE: " << program_binary.size() << endl;
    cl::Program::Binaries bins{{program_binary.data(), program_binary.size()}};
    
    // meta files about bins
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

    q.enqueueMigrateMemObjects({buffer_a, buffer_b}, 0 ); /* 0 means from host*/

    //***************************************************
    // STEP 8: Set the kernel arguments
    //***************************************************

    kernel.setArg(0, buffer_res);
    kernel.setArg(1, buffer_a);
    kernel.setArg(2, buffer_b);
    kernel.setArg(3, DATA_SIZE);

    //***************************************************
    // STEP 9: Enqueue the kernel for execution
    //***************************************************

    q.enqueueTask(kernel);

    //***************************************************
    // STEP 12: Read the output buffer back to the host
    //***************************************************

    q.enqueueMigrateMemObjects({buffer_res}, CL_MIGRATE_MEM_OBJECT_HOST);

    q.finish();

    cout << "Calculating sum of resulting array: " << endl;
    
    int sum = 0;
    for (int i = 0; i < DATA_SIZE; i++){
        sum += c[i];
    }

    cout << "SUM: " << sum << endl;
    
    return 0;
}
