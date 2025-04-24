#!/bin/bash
if [ $# -lt 2 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados y el nombre del dataset: (MH01, V203, ...)"
else
	sudo cpupower -c all frequency-set -d "1600MHz" -u "1600MHz"
	sudo cpupower monitor -c all --policy
	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 3 5 ${1}_"1600MHz" #33ms frame limit --- 4-> Latencia total medida 5-> Coste energético
	#./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 $numTokens 2 ${1}_$myFreq #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
	
	sudo cpupower -c all frequency-set -d "2400MHz" -u "2400MHz"
	sudo cpupower monitor -c all --policy
	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 4 5 ${1}_"2400MHz" #33ms frame limit --- 4-> Latencia total medida 5-> Coste energético

  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
