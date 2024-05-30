#!/bin/bash
if [ $# -lt 2 ]; then
	echo "You need the name of the dataset. Belongs to this list: (MH01, MH02, MH03, MH04, MH05, V101, V102, V103, V201, V202, V203)"
	echo "You also need to specify the number of tokens to use."
	exit 1
fi

dataset=$1
N_TOKENS_PIPELINE=$2
res_directory=Results/${dataset}_${N_TOKENS_PIPELINE}
ORIGINAL_PATH=$(pwd)
DATASETS_PATH=/mnt/extra/jferrer/Datasets

echo ""
echo "***********************************************************************"
echo CALL FOR DATASET $dataset with $N_TOKENS_PIPELINE TOKENS IN THE PIPELINE
echo "***********************************************************************"
echo ""

rm -fr $res_directory
mkdir -p $res_directory
cd $res_directory
T_START=$SECONDS
$ORIGINAL_PATH/Examples/Stereo/stereo_euroc $N_TOKENS_PIPELINE $N_TOKENS_PIPELINE $ORIGINAL_PATH/Vocabulary/ORBvoc.txt $ORIGINAL_PATH/Examples/Stereo/EuRoC.yaml $DATASETS_PATH/EuRoC/$dataset $ORIGINAL_PATH/Examples/Stereo/EuRoC_TimeStamps/$dataset.txt data_orbslam > orbslam3_output.log 2>&1
T_ELAPSED=$(($SECONDS-$T_START))
if [ -f data_orbslam_f.txt ]; then
  echo $T_ELAPSED
	date -d@$T_ELAPSED -u +%M:%S
	echo -n "ATE,scale,GT_ATE\n" >> SessionInfo.txt
	python $ORIGINAL_PATH/evaluation/evaluate_ate_scale.py $ORIGINAL_PATH/evaluation/Ground_truth/EuRoC_left_cam/${dataset}_GT.txt ./data_orbslam_f.txt >> SessionInfo.txt
	echo -n "Number of words in the data: " >> SessionInfo.txt
  ls $DATASETS_PATH/EuRoC/${dataset}/mav0/cam0/data | wc -l >> SessionInfo.txt

	cat SessionInfo.txt
fi
cd $ORIGINAL_PATH
echo ""
echo "***********************************************************************"
echo END OF SCRIPT. PLEASE CHECK OUTPUTS IN THE RESULTS FOLDER $res_directory
echo "***********************************************************************"
echo ""
