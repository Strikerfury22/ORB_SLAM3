#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  # "800MHz" "1600MHz" "2400MHz" "3200MHz" 
  for myFreq in "4000MHz"
  do
    #for nTokens in {1..8}
    #do
      #echo $myFreq
      sudo cpupower -c all frequency-set -d $myFreq -u $myFreq
      sudo cpupower monitor -c all --policy
      ./my_launch_files/hoGAZa/launch_calls_perf.sh MH01 5 4 ${1}_${myFreq}_noTimes_idealCam
      ./my_launch_files/hoGAZa/launch_calls_perf.sh MH01 5 3 ${1}_${myFreq}_times_idealCam
    #done
  done
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
