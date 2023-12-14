# Using Vitis HLS and analyzing reports


## Goal:
In this exercise, we will utilize Vitis HLS to synthesize the kernel. To facilitate this process, we have prepared the ***run_hls.tcl*** script in the ***src*** directory, which establishes the environment and initiates synthesis. 

## Instructions: 

- move to subdirectory:
```console
$ cd 04-matv_mul-hls
```

### SLING NSC cluster: 


- To execute the Tcl script, run the following shell script.
```sh linenums="1"
./run_hls.sh SLING
```

- After a successful synthesis, you can locate the results in the file.
```sh linenums="1"
./build/project/solution/syn/report/cynth.rpt 
```

### IT4I CS cluster: 

- To execute the Tcl script, run the following shell script.
```sh linenums="1"
./run_hls.sh IT4I
```

- After a successful synthesis, you can locate the results in the file.
```sh linenums="1"
./build/project/solution/syn/report/cynth.rpt 
```


