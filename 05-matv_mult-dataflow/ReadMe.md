# Dataflow optimization[^1]


## Goal:
In this exercise, we will employ dataflow optimization and analyze the HLS results. 

## Instructions: 

- move to subdirectory:
```console
$ cd 05-matv_mul-dataflow
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


___

[^1]: 
 &copy; Nejc Ilc and Ratko Pilipović, University of Ljubljana, Faculty of Computer and Information Science. 
 The materials are published under the ßlicense [Attribution-NonCommercial-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-nc-sa/4.0/).

___
