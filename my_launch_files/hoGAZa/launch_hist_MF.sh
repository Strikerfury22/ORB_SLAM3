#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  for myFreq in "800MHz" "1600MHz" "2400MHz" "3200MHz" "4000MHz"
  do
	  #echo $myFreq
	  sudo cpupower -c all frequency-set -d $myFreq -u $myFreq
	  sudo cpupower monitor -c all --policy
	  for numTokens in 8
	#  for numTokens in {1..7}
	  do
		./my_launch_files/hoGAZa/launch_calls_euroc.sh MH01 $numTokens 4 ${1}_$myFreq
		#./my_launch_files/hoGAZa/launch_calls_euroc.sh MH01 8 0 ${1}_4000MHz
	  done
  done
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
