#!/usr/bin/env zsh
#SBATCH --partition=instruction
#SBATCH --time=00:01:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --gpus-per-task=1
#SBATCH --output=mm_coalesced.output

module load nvidia/cuda
nvcc mm_coalesced.cu -o mm_coalesced
./mm_coalesced
