#!/bin/bash
# IF statement if shell which determines the paths for xrt and vivado
# one branch for NSC one branch for ITI4CS 
PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1
#PLATFORM=xilinx_u280_gen3x16_xdma_1_202211_1


if test $1 = "IT4I"
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
    #module load foss/2022a
fi


HOME=$PWD
DIR=../src
BUILD_DIR=./build
mkdir -p $BUILD_DIR


KERNEL=matv_mult_dataflow
OBJ_FILE=${KERNEL}_$1

cp ./src/xrt.ini $BUILD_DIR/xrt.ini

cd $BUILD_DIR
pwd


g++ $DIR/host.cpp -I$XILINX_XRT/include -I$XILINX_VIVADO/include -lOpenCL -pthread -o ./host


cd $HOME
