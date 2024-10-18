#!/bin/bash
if [ $# -lt 3 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados, el numero de tokens y el modo. En ese orden."
  echo "Los modos son:"
  echo "0 -> tbb sin tiempos"
  echo "1 -> thread sin tiempos"
  echo "2 -> tbb con tiempos"
  echo "3 -> thread con tiempos"
  
else
  ./my_launch_files/hoGAZa/launch_calls_perf.sh MH01 $2 $3 $1
fi
