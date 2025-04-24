#!/bin/bash
if [ $# -lt 3 ]; then
	echo "You need the name of the dataset. Belongs to this list: (MH01, MH02, MH03, MH04, MH05, V101, V102, V103, V201, V202, V203)"
	echo "You also need to specify the number of tokens to use."
  echo "Lastly, you need to pass the mode."
  echo "The modes are:"
  echo "0 -> tbb without times"
  echo "1 -> thread without times"
  echo "2 -> tbb with times"
  echo "3 -> thread with times"
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
#for i in {1..3}
for i in {1..15}
#for i in {1..12}
do
  echo ""
  echo "***********************************************************************"
  echo CALL $i FOR DATASET $dataset with $N_TOKENS_PIPELINE TOKENS IN THE PIPELINE
  echo "***********************************************************************"
  echo ""
  res_directory=2025/Results_22_01/${dataset}_${N_TOKENS_PIPELINE}_${extResDir}_$i
  
  rm -fr $res_directory
  mkdir -p $res_directory
  cd $res_directory
  touch SessionInfo.txt
  T_START=$SECONDS
  if [ $3 -eq 0 ]; then
    sudo perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_tbb_no_times $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
  elif [ $3 -eq 1 ]; then
    sudo perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_threads_no_times $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
  elif [ $3 -eq 2 ]; then
    sudo perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_tbb_times $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
  elif [ $3 -eq 3 ]; then
    nice -20 sudo perf stat -e power/energy-cores/,power/energy-ram/,power/energy-pkg/ $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_hist_full_latency $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
  elif [ $3 -eq 4 ]; then
    nice -20 sudo $ORIGINAL_PATH/Examples/Stereo/stereo_euroc_hist_full_latency $N_TOKENS_PIPELINE 27 $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
  fi
  T_ELAPSED=$(($SECONDS-$T_START))
  if [ -f data_orbslam_f.txt ]; then
    echo $T_ELAPSED
  	date -d@$T_ELAPSED -u +%M:%S
  	sudo echo -n "ATE,scale,GT_ATE\n" >> SessionInfo.txt
  	sudo python $ORIGINAL_PATH/evaluation/evaluate_ate_scale.py $ORIGINAL_PATH/evaluation/Ground_truth/EuRoC_left_cam/${dataset}_GT.txt ./data_orbslam_f.txt >> SessionInfo.txt
  	sudo echo -n "Number of words in the data: " >> SessionInfo.txt
    sudo ls $DATASETS_PATH/${dataset}/mav0/cam0/data | wc -l >> SessionInfo.txt
  
  	cat SessionInfo.txt
  fi
  cd $ORIGINAL_PATH
  echo ""
  echo "***********************************************************************"
  echo END OF SCRIPT. PLEASE CHECK OUTPUTS IN THE RESULTS FOLDER $res_directory
  echo "***********************************************************************"
  echo ""
done
