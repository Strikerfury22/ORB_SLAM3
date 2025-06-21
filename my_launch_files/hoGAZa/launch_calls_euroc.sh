#!/bin/bash
if [ $# -lt 3 ]; then
  echo "You need the name of the dataset. Belongs to this list: (MH01, V203)"
  echo "You also need to specify the number of tokens to use."
  echo "Lastly, you need to pass the mode."
  echo "The modes are:"
  echo "0 -> ideal camera measure latency (old)"
  echo "1 -> ideal camera measure time"
  echo "2 -> ideal camera energy"
  echo "3 -> ideal camera measure latency and energy"
  echo "4 -> 30FPS camera measure latency"
  echo "5 -> 30FPS camera measure latency and energy"
  echo "6 -> ideal camera with time, energy and latency measurements and free_mutex_free final division"
  echo "7 -> ideal camera with full measurements (time, energy, latency and traces) and free_mutex_free final division"
  echo "8 -> ideal camera with time, energy and latency (both section and full) and free_mutex_free final division"
  echo "You can also name the directory to save the output with a fourth argument"
	exit 1
fi

if [ $# -eq 4 ]; then
	extResDir=$4
else
	extResDir="_default"
fi

dataset=$1
N_TOKENS_PIPELINE=$2
ORIGINAL_PATH=$(pwd)
DATASETS_PATH=/home/gaz/Desktop/MarcosIlarraza/datasets
for i in {1..3}
do
  echo ""
  echo "***********************************************************************"
  echo CALL $i FOR DATASET $dataset with $N_TOKENS_PIPELINE TOKENS IN THE PIPELINE
  echo "***********************************************************************"
  echo ""
  res_directory=2025/Results_04_06_2025/${dataset}_${N_TOKENS_PIPELINE}_${extResDir}_$i #Results_20_12_2024 #Results_12_05_2025
  while [ ! -f $res_directory/data_orbslam_f.txt ]; 
  do # Solo ejecuta si no faltan los resultados
    echo "Intentamos generar resultados"
    rm -fr $res_directory
    mkdir -p $res_directory
    cd $res_directory
    touch SessionInfo.txt
    T_START=$SECONDS
    if [ $3 -eq 0 ]; then
      nice -20 $ORIGINAL_PATH/Examples/Stereo/stereo_hist_latency $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 1 ]; then
      $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_threads_times $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 2 ]; then
      sudo perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ nice -20 $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_threads_no_times $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 3 ]; then
      sudo nice -20 perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_hist_full_latency $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 4 ]; then
      nice -20 $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_hist_33ms $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 5 ]; then
      sudo nice -20 perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_hist_33ms $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 6 ]; then
      sudo nice -20 perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_free_mutex_free $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 7 ]; then
      sudo nice -20 perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_free_mutex_free_traces $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    elif [ $3 -eq 8 ]; then
      sudo nice -20 perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_free_mutex_free_sections $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
    fi
    echo "Ejecucion Acabada"
    T_ELAPSED=$(($SECONDS-$T_START))
    if [ -f data_orbslam_f.txt ]; then
      echo $T_ELAPSED >> execTime.txt
    	date -d@$T_ELAPSED -u +%M:%S
    	sudo echo -n "ATE,scale,GT_ATE\n" >> SessionInfo.txt
    	sudo python $ORIGINAL_PATH/evaluation/evaluate_ate_scale.py $ORIGINAL_PATH/evaluation/Ground_truth/EuRoC_left_cam/${dataset}_GT.txt ./data_orbslam_f.txt >> SessionInfo.txt
    	sudo echo -n "Number of words in the data: " >> SessionInfo.txt
      sudo ls $DATASETS_PATH/${dataset}/mav0/cam0/data | wc -l >> SessionInfo.txt
    
    	cat SessionInfo.txt
    fi
    cd $ORIGINAL_PATH
  done
  
  echo ""
  echo "***********************************************************************"
  echo END OF SCRIPT. PLEASE CHECK OUTPUTS IN THE RESULTS FOLDER $res_directory
  echo "***********************************************************************"
  echo ""
done
