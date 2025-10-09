#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  sudo cpupower -c all frequency-set -d 4000MHz -u 4000MHz
  sudo cpupower monitor -c all --policy
  sudo ldconfig /home/gaz/Desktop/MarcosIlarraza/orb_slam_libs/lib # Muchas pruebas han fallado por culpa de esto
  ./my_launch_files/hoGAZa/launch_calls_euroc.sh MH01 8 7 ${1}_ideal_4000MHz
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
