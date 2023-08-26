#!/bin/bash

list=(MH01 MH02 MH03 MH04 MH05 V101 V102 V103 V201 V202 V203)

if [ $# -ne 2 ]; then
    echo "Need two arguments [results_dir n_runs]"
    exit 1
fi

RES_DIR=$1
N_RUNS=$2
ORIGINAL_PATH=$(pwd)
DATASETS_PATH=/mnt/extra/jferrer/Datasets

for i in "${list[@]}"
do
    echo "*******************************************************"
    echo COMPUTING DATASET $i
    echo "*******************************************************"
    echo ""
    for n in $(seq $N_RUNS)
    do
        cd $ORIGINAL_PATH
        echo -en "\tRun $n/$N_RUNS\t"

        dest_path=$RES_DIR/$i/run_$n
        mkdir -p $dest_path
        cd $dest_path
        T_START=$SECONDS
        $ORIGINAL_PATH/Examples/Stereo/stereo_euroc $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/EuRoC/$i $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$i.txt data_orbslam > orbslam3_output.log 2>&1
        T_ELAPSED=$(($SECONDS-$T_START))

        date -d@$T_ELAPSED -u +%M:%S
        echo -n "ATE,scale,GT_ATE: " >> SessionInfo.txt
        python $ORIGINAL_PATH/evaluation/evaluate_ate_scale.py $ORIGINAL_PATH/evaluation/Ground_truth/EuRoC_left_cam/${i}_GT.txt ./data_orbslam_f.txt >> SessionInfo.txt
        echo -n "Num Frames: " >> SessionInfo.txt
        ls $DATASETS_PATH/EuRoC/${i}/mav0/cam0/data | wc -l >> SessionInfo.txt

        cat ./SessionInfo.txt
    done


    #echo $i
    #ls ./evaluation/Ground_truth/EuRoC_left_cam/${i}_GT.txt
    
    
    
    echo ""
done


#python $base_path/evaluation/evaluate_ate_scale.py $base_path/evaluation/Ground_truth/EuRoC_left_cam/MH01_GT.txt \
#dataset-MH01_stereo_f.txt --plot MH01_stereo.pdf


