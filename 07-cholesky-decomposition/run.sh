PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1

if test $2 = "IT4I"
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
    module load foss/2022a
fi


cd ./build
DIR=./src
BUILD_DIR=./build
KERNEL=cho_dec

XCL_EMULATION_MODE=$1 ./host $PLATFORM $KERNEL.xclbin

cd $HOME