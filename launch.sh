#!/bin/bash

if [ $# -eq 0 ]; then
    echo "No results subdirectory supplied"
    exit 1
fi

base_path=$(pwd)
res_path="Results/$1"

# mkdir -p $res_path

# runs=($(find $res_path -type d -name "run_*"))

# numbers=($(echo "${runs[@]}" | grep -oP '(?<=run_)\d+'))
# largest=$(echo "${numbers[@]}" | tr ' ' '\n' | sort -nr | head -1)
# next_number=$((largest+1))

# run_path="$res_path/run_$next_number"

# mkdir $run_path
# cd $run_path

# $base_path/Examples/Stereo/stereo_euroc $base_path/Vocabulary/ORBvoc.txt $base_path/Examples/Stereo/EuRoC.yaml \
# $base_path/Datasets/EuRoC/MH01 $base_path/Examples/Stereo/EuRoC_TimeStamps/MH01.txt dataset-MH01_stereo

cd run_path

python $base_path/evaluation/evaluate_ate_scale.py $base_path/evaluation/Ground_truth/EuRoC_left_cam/MH01_GT.txt \
dataset-MH01_stereo_f.txt --plot MH01_stereo.pdf


