#!/usr/bin/env bash
####### Partition #######
#SBATCH --partition=all

####### Resources #######
#SBATCH --time=0-00:05:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0001542/out/pi_%x.%j.out
#SBATCH --error=/home/fd0001542/out/pi_%x.%j.err

cd /home/fd0001542/parl_comp/ex5

# Parameters passed via sbatch --export
variant=${VARIANT:-seq}
threads=${THREADS:-1}
chunk=${CHUNK:-0}

echo "Running experiment: variant=${variant} threads=${threads} chunk=${chunk}"
./pi_par_loop "${variant}" "${threads}" "${chunk}"


