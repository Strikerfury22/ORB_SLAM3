#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  for nTokens in {1..14}
  do
    for myFreq in "800MHz" "1600MHz" "2400MHz" "3200MHz" "4000MHz"
    do
      #echo $myFreq
      sudo cpupower -c all frequency-set -d $myFreq -u $myFreq
      sudo cpupower monitor -c all --policy
      ./my_launch_files/hoGAZa/launch_calls_perf.sh MH01 $nTokens 1 ${1}_${myFreq}
    done
  done
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
