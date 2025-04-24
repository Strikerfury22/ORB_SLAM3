#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  for myFreq in "4000MHz" "800MHz" "1600MHz" "2400MHz" "3200MHz"
  do
    #echo $myFreq
    sudo cpupower -c all frequency-set -d $myFreq -u $myFreq
    sudo cpupower monitor -c all --policy
    ./my_launch_files/hoGAZa/malagaDataset/launch_calls_malaga.sh Malaga07_unrectified 8 0 ${1}_${myFreq}
  done
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
