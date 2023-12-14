#!/bin/bash
#SBATCH --job-name=hw_synthesis 
#SBATCH --output=output_hwSynthesis.txt
#SBATCH --error=error_hwSynthesis.txt
#SBATCH --time=24:00:00


srun bash compile_kernel_u280.sh hw
echo "hw synthesis done for u280"
srun bash compile_kernel_u250.sh hw
echo "hw synthesis done for u280"
