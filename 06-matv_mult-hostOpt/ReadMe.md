# Exercise 1: Matrix-vector multiplication - Naive approach[^1]

## Goal:

The purpose of this exercise is to adapt the host code to employ multiple compute units. 

## Instructions:

- move to subdirectory:
```console
$ cd 05-matv_mul-hostOpt
```
- build the host application  

```console
$ ./compile_host.sh SLING # or IT4I
```

- build the kernel
```console
$ ./compile_kernel.sh sw_emu SLING # or IT4I
```console

- Run the application:
```console
$ ./run.sh sw_emu SLING # or IT4I
```

- Analyze the results:

On SLING NSC cluster: 

```console
. /ceph/grid/software/xilinx/tools/Vitis/2023.1/settings64.sh
vitis_analyzer ./build/
```
On IT4I CS cluster: 

```console
. /tools/Xilinx/Vitis/2023.1/settings64.sh
vitis_analyzer ./build/
```
___

[^1]: 
 &copy; Nejc Ilc and Ratko Pilipović, University of Ljubljana, Faculty of Computer and Information Science. 
 The materials are published under the license [Attribution-NonCommercial-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-nc-sa/4.0/).

___
