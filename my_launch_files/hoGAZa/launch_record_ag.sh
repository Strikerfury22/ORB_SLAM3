#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  #echo $myFreq
  sudo cpupower -c all frequency-set -d 4000MHz -u 4000MHz
  sudo cpupower monitor -c all --policy
  ./my_launch_files/hoGAZa/launch_perfA.sh MH01 8 0 ${1}_full_latency
  ./my_launch_files/hoGAZa/launch_perfA.sh MH01 8 1 ${1}_section_latency
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
