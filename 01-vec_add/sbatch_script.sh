#!/bin/bash
#SBATCH --job-name=hw_synthesis 
#SBATCH --output=output_hwSynthesis.txt
#SBATCH --error=error_hwSynthesis.txt
#SBATCH --time=01:00:00

srun bash run.sh sw_emu IT4I
