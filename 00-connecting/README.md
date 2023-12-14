# Setting up the environment[^1]


## Connecting to SLING NSC cluster

- Open the terminal and connect to the cluster using SSH

```console
$ ssh -X username-ID@nsc-login2.ijs.si
```

- Allocate the node with FPGA 

```console
$ salloc -N1 -c2 -C fpga --reservation=fpga --time=01:00:00
```

- Connect to it

```console
$ ssh -X nsc-vfp001
```

- Find the FPGA devices
```
$ . /ceph/grid/software/xilinx/tools/Vitis/2023.1/settings64.sh
$ . /opt/xilinx/xrt/setup.sh
$ xbutil examine 
```


## Connecting to IT4I CS cluster 

- Open the terminal and connect to the cluster using SSH

```console
$ ssh -X username-ID@login.cs.it4i.cz
```

- Allocate and connect to the node with FPGA 

```console
$ salloc --x11 -A DD-23-117 -c2 -p p03-amd --gres=fpga 
```

- Find the FPGA devices
```
$ . /tools/Xilinx/Vitis/2023.1/settings64.sh
$ . /opt/xilinx/xrt/setup.sh
$ xbutil examine 
```
## Batch scripts 


### SLING NSC cluster

- Open the terminal and connect to the cluster using SSH

```console
$ sbatch -c2 -C fpga --reservation=fpga ./sbatch_script.sh 
```


### IT4I CS cluster 

- Open the terminal and connect to the cluster using SSH

```console
$ sbatch -A DD-23-117 -c2 -p p03-amd --gres=fpga ./sbatch_script.sh 
```




___

[^1]: 
 &copy; Nejc Ilc and Ratko Pilipović, University of Ljubljana, Faculty of computer and information science. 
 The materials are published under license [Attribution-NonCommercial-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-nc-sa/4.0/).

___
