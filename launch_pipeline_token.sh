#!/bin/bash

for i in {25..30}
do
    echo NUMBER TOKENS $i
    ./launch_missing.sh Results/pipeline_token_parallel/$i 3 $i 
done
