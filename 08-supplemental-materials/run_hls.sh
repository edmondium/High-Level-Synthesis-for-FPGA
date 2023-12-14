
#!/bin/bash
# IF statement if shell which determines the paths for xrt and vivado
# one branch for NSC one branch for ITI4CS 
PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1

IT4I_HOST="login.cs.it4i.cz"
if test "$HOSTNAME" = "$IT4I_HOST"
then
    # Execute commands for hostname equal to str1
    echo "you are on login.cs.it4i.cz"
    export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/${PLATFORM}/
    . /tools/Xilinx/Vitis/2023.1/settings64.sh
    . /opt/xilinx/xrt/setup.sh
else
    # Execute commands for hostname not equal to str1
    echo "you are on nsc"
    . /ceph/grid/software/xilinx/tools/Vitis/2023.1/settings64.sh
    . /opt/xilinx/xrt/setup.sh
    export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/${PLATFORM}/
fi


HOME=$PWD
DIR=../src
BUILD_DIR=./build
mkdir -p $BUILD_DIR


cp ./src/xrt.ini $BUILD_DIR/xrt.ini
cp ./src/run_hls.tcl $BUILD_DIR/run_hls.tcl 

cd $BUILD_DIR

vitis_hls -f run_hls.tcl

cd $HOME