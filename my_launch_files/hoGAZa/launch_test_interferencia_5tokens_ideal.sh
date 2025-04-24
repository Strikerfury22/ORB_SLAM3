#!/bin/bash
if [ $# -lt 2 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados y el nombre del dataset: (MH01, V203, ...)"
else
  for myFreq in "800MHz" "1600MHz" "2400MHz" "3200MHz" "4000MHz"
  do
	  #echo $myFreq
	  sudo cpupower -c all frequency-set -d $myFreq -u $myFreq
	  sudo cpupower monitor -c all --policy
	  for numTokens in 5
	  #for numTokens in {1..8}
	  do
		#./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 $numTokens 5 ${1}_$myFreq #33ms frame limit --- 4-> Latencia total medida 5-> Coste energético
		#./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 $numTokens 3 hist_${1}_$myFreq #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
		./my_launch_files/hoGAZa/launch_calls_euroc.sh $2 $numTokens 2 noHist_${1}_$myFreq #no frame limit ---  0-> Latencia total medida 2-> Coste energetico
	  done
  done
  sudo cpupower -c all frequency-set -d 800MHz -u 4000MHz
fi
