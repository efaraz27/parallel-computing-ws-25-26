#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="pi_par_loop_experiments"

####### Partition #######
#SBATCH --partition=all

####### Resources #######
#SBATCH --time=0-00:10:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0001542/out/pi_par_loop.out.%j
#SBATCH --error=/home/fd0001542/out/pi_par_loop.err.%j

cd /home/fd0001542/parl_comp/ex5

srun --ntasks=1 ./pi_par_loop seq 1 0

threads_list=(1 2 4 8)
chunks=(0 1 10 100 1000)

for t in "${threads_list[@]}"; do
  for c in "${chunks[@]}"; do
    srun --ntasks=1 ./pi_par_loop static "$t" "$c"
  done
done

for t in "${threads_list[@]}"; do
  for c in "${chunks[@]}"; do
    srun --ntasks=1 ./pi_par_loop dynamic "$t" "$c"
  done
done

for t in "${threads_list[@]}"; do
  for c in "${chunks[@]}"; do
    srun --ntasks=1 ./pi_par_loop guided "$t" "$c"
  done
done