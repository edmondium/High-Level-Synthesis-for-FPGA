
#!/bin/bash
# IF statement if shell which determines the paths for xrt and vivado
# one branch for NSC one branch for ITI4CS 
PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1
#PLATFORM=xilinx_u280_gen3x16_xdma_1_202211_1

echo "you are on login.cs.it4i.cz"
export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/${PLATFORM}/
. /tools/Xilinx/Vitis/2023.1/settings64.sh
. /opt/xilinx/xrt/setup.sh

HOME=$PWD
DIR=../src
BUILD_DIR=./build
mkdir -p $BUILD_DIR


KERNEL=matv_mult_dataflow

cp ./src/xrt.ini $BUILD_DIR/xrt.ini

cd $BUILD_DIR
pwd
#OBJ_FILE=${KERNEL}_u280_$1
OBJ_FILE=${KERNEL}_u250_$1
CU_NUM=4
emconfigutil --platform $PLATFORM
v++ -c -t $1 --connectivity.nk $KERNEL:$CU_NUM --platform $PLATFORM --config $DIR/fpga.cfg -k $KERNEL $DIR/$KERNEL.cpp -o ./$OBJ_FILE.xo
v++ -l -t $1 --connectivity.nk $KERNEL:$CU_NUM --platform $PLATFORM --config $DIR/fpga.cfg ./$OBJ_FILE.xo -o ./$OBJ_FILE.xclbin


cd $HOME