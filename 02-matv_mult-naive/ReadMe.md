# Exercise 1: Matrix-vector multiplication - Naive approach

## Goal:

The purpose of this exercise is to implement a simple matrix-vector multiplication kernel on the FPGA. 

## Instructions:

- move to subdirectory:
```console
$ cd 02-matv_mul-naive
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

