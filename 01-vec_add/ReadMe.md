# Exercise 1: vector sum

## Goal:

The purpose of this exercise is to run our first application, which uses an FPGA to compute the sum of two vectors.

## Instructions:

- move to subdirectory:
```console
$ cd 01-vec_add/
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
