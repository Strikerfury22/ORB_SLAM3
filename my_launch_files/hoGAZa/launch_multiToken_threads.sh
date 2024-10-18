#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  for nTokens in {1..8}
  do
    #echo $nTokens # Depurar el funcionamiento del bucle for
    ./my_launch_files/hoGAZa/launch_single_call_threads.sh MH01 $nTokens $1
  done
fi
