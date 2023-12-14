# Exercise 1: Matrix-vector multiplication - Optimized version 

## Goal:

The purpose of this exercise is to implement a matrix-vector multiplication kernel with optimized memory access on the FPGA. 

## Instructions:

- move to subdirectory:
```console
$ cd 03-matv_mul-opt
```
### SLING NSC cluster: 

- build the application  

```console
$ ./compile.sh sw_emu SLING # or hw_emu for hw emulation
```

- Run the application:
```console
$ ./run.sh sw_emu SLING #or hw_emu for hw emulation
```
- Analyze the results 

```console
$ . /ceph/grid/software/xilinx/tools/Vitis/2023.1/settings64.sh
$ vitis_analyzer ./build/
```

### IT4I CS cluster: 

- build the application  

```console
$ ./compile.sh sw_emu IT4I # or hw_emu for hw emulation
```

- Run the application:
```console
$ ./run.sh sw_emu IT4I #or hw_emu for hw emulation
```
- Analyze the results 

```console
. /tools/Xilinx/Vitis/2023.1/settings64.sh
vitis_analyzer ./build/
```
