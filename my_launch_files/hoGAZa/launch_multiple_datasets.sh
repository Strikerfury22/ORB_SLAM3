#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  #echo $myFreq
  for myFreq in "800MHz" "1600MHz" "2400MHz" "3200MHz" "4000MHz"
  do
    sudo cpupower -c all frequency-set -d $myFreq -u $myFreq
    sudo cpupower monitor -c all --policy
    for dataset in "MH02" "MH03" "MH04" "MH05" "V101" "V102" "V103" "V201" "V202" "V203"
    do
      for nTokens in {1..14}
      do
        sudo ldconfig /home/gaz/Desktop/MarcosIlarraza/orb_slam_libs/lib # Muchas pruebas han fallado por culpa de esto
        ./my_launch_files/hoGAZa/launch_calls_euroc.sh $dataset $nTokens 3 ${1}_ideal_${myFreq}
        ./my_launch_files/hoGAZa/launch_calls_euroc.sh $dataset $nTokens 5 ${1}_30FPS_${myFreq}
      done
    done
  done
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
