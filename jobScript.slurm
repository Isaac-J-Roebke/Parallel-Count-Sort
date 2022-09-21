#!/bin/bash

#SBATCH --ntasks=12 
#SBATCH --time=2:00:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0268
#SBATCH --job-name=Roebke_Isaac_R03

export OMP_NUM_THREADS=12
cd $SLURM_SUBMIT_DIR/Default
make all
./OpenMP > results.csv
