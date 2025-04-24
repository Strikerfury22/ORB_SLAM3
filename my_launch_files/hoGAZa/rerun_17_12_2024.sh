#!/bin/bash
if [ $# -lt 2 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados y el nombre del dataset: (MH01, V203, ...)"
else
#	sudo cpupower -c all frequency-set -d "4000MHz" -u "4000MHz"
#	sudo cpupower monitor -c all --policy
#	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 1 2 ${1}_"4000MHz" #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
#	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 4 2 ${1}_"4000MHz" #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
	
#	sudo cpupower -c all frequency-set -d "2400MHz" -u "2400MHz"
#	sudo cpupower monitor -c all --policy
#	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 4 2 ${1}_"2400MHz" #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
#	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 7 2 ${1}_"2400MHz" #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
	
	sudo cpupower -c all frequency-set -d "3200MHz" -u "3200MHz"
	sudo cpupower monitor -c all --policy
	./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 8 2 ${1}_"3200MHz" #no frame limit ---  0-> Latencia total medida 2-> Coste energetico

  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
