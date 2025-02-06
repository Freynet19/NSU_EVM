#!/bin/bash

n_arr=(5000000000 7500000000 10000000000 12500000000)

for oflag in "-O0" "-O1" "-O2" "-O3" "-Ofast" "-Os" "-Og"
do
  echo "Optimization flag: $oflag"
  gcc lab2.c $oflag -o main.out
  for i in "${n_arr[@]}"
  do
    echo "N = $i"
    ./main.out $i
  done
  echo "____________________________________"
done
