#!/usr/bin/env bash

cd /home/fd0001542/parl_comp/ex5

echo "Submitting sequential baseline..."
sbatch --export=ALL,VARIANT=seq,THREADS=1,CHUNK=0 ./pi_experiment.sh

threads_list=(1 2 4 8)
chunks=(0 1 10 100 1000)

for variant in static dynamic guided; do
  for t in "${threads_list[@]}"; do
    for c in "${chunks[@]}"; do
      echo "Submitting experiment: variant=${variant} threads=${t} chunk=${c}"
      sbatch --export=ALL,VARIANT=${variant},THREADS=${t},CHUNK=${c} ./pi_experiment.sh
    done
  done
done


