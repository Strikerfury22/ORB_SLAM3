#!/bin/bash

grainsize=1
max_value=1500
until ((grainsize > max_value)); do
    echo $grainsize
    ./launch_missing.sh Results/grainsizePF1/$grainsize 3 $grainsize
    grainsize=$((2*grainsize))
done