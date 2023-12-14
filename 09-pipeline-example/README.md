# Supplemental materials 

### HLS synthesis 

In this exercise, we will utilize Vitis HLS to synthesize the kernel. To facilitate this process, we have prepared the ***run_hls.tcl*** script in the ***src*** directory, which establishes the environment and initiates synthesis. To execute the Tcl script, run the following shell script.

```sh linenums="1"
./run_hls.sh
```

after a successful synthesis, you can locate the results in the file.
```sh linenums="1"
./build/project-vadd/solution/syn/report/cynth.rpt 
```
