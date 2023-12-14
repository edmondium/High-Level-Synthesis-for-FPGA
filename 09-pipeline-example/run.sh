PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1
#PLATFORM=xilinx_u280_gen3x16_xdma_base_1


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

cd ./build
DIR=./src
BUILD_DIR=./build
KERNEL=./matv_mult_dataflow
ls
#OBJ_FILE=${KERNEL}_$1

OBJ_FILE=${KERNEL}_$1
OOO=0

echo $OBJ_FILE
XCL_EMULATION_MODE=$1 ./host $PLATFORM $OBJ_FILE.xclbin 
#./host $PLATFORM $OBJ_FILE.xclbin 

cd $HOME