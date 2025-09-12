#!/usr/bin/env zsh

# Requests the instruction partition
# Runs for a maximum of 1 minute
# Uses 1 CPU core
# Prints Hello from node <hostname> to the output file

#SBATCH --partition=instruction
#SBATCH --time=00:01:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --output=hello.output
cd $SLURM_SUBMIT_DIR
echo "Hello from node $(hostname)"
