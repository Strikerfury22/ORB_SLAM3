#!/bin/bash

list=(MH01 MH02 MH03 MH04 MH05 V101 V102 V103 V201 V202 V203)

if [ $# -lt 2 ] || [ $# -gt 3 ]; then
    echo "Need two or three arguments [results_dir n_runs (number_tokens_pipeline)]"
    exit 1
fi

RES_DIR=$1
N_RUNS=$2
N_TOKENS_PIPELINE=$3
ORIGINAL_PATH=$(pwd)
DATASETS_PATH=/mnt/extra/jferrer/Datasets

excepcion_counter=()
for i in "${list[@]}"; do
    excepcion_counter+=(0)
done


for i in "${!list[@]}"
do
    dataset=${list[$i]}
    echo ""
    echo "*******************************************************"
    echo COMPUTING DATASET $dataset
    echo "*******************************************************"
    echo ""
    for n in $(seq $N_RUNS)
    do
        cd $ORIGINAL_PATH
        dest_path=$RES_DIR/$dataset/run_$n

        if [ ! -f $dest_path/data_orbslam_f.txt ]; then #One computation is going to be needed
            excepcion_counter[$i]=$((excepcion_counter[$i] - 1))
        else
            echo -e "\tRun $n\tOk!"
        fi

        while [ ! -f $dest_path/data_orbslam_f.txt ]; do #Continuosly check until correctly generated
            echo -en "\tRun $n\t"
            rm -fr $dest_path
            mkdir -p $dest_path
            cd $dest_path
            T_START=$SECONDS
            if [ -z "$N_TOKENS_PIPELINE" ]; then #Not provided
                $ORIGINAL_PATH/Examples/Stereo/stereo_euroc $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/EuRoC/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
            else #Provided
                $ORIGINAL_PATH/Examples/Stereo/stereo_euroc 20 $N_TOKENS_PIPELINE $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/EuRoC/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
            fi
            T_ELAPSED=$(($SECONDS-$T_START))

            if [ -f data_orbslam_f.txt ]; then #Only do this when computation succesful to avoid verbose mess
                date -d@$T_ELAPSED -u +%M:%S
                echo -n "ATE,scale,GT_ATE: " >> SessionInfo.txt
                python $ORIGINAL_PATH/evaluation/evaluate_ate_scale.py $ORIGINAL_PATH/evaluation/Ground_truth/EuRoC_left_cam/${dataset}_GT.txt ./data_orbslam_f.txt >> SessionInfo.txt
                echo -n "Num Frames: " >> SessionInfo.txt
                ls $DATASETS_PATH/EuRoC/${dataset}/mav0/cam0/data | wc -l >> SessionInfo.txt

                cat ./SessionInfo.txt
            fi

            
            cd $ORIGINAL_PATH #Necesary to re-check at the correct local path
            excepcion_counter[$i]=$((excepcion_counter[$i] + 1))
        done
    done
done

echo ""
echo "*******************************************************"
echo SESION SUMMARY
echo "*******************************************************"
echo ""
for i in "${!list[@]}"
do
    echo -e "Dataset ${list[$i]}\t ${excepcion_counter[$i]} excepcions"
done


#python $base_path/evaluation/evaluate_ate_scale.py $base_path/evaluation/Ground_truth/EuRoC_left_cam/MH01_GT.txt \
#dataset-MH01_stereo_f.txt --plot MH01_stereo.pdf


