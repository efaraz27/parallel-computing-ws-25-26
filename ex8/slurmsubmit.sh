#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="fib_experiments"

####### Partition #######
#SBATCH --partition=all

####### Resources #######
#SBATCH --time=0-00:10:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0001542/out/fib.out.%j
#SBATCH --error=/home/fd0001542/out/fib.err.%j

cd /home/fd0001542/parl_comp/ex8

srun ./fib 10 20