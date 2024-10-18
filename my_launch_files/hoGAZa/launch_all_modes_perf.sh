#!/bin/bash
if [ $# -lt 2 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados y el numero de tokens. En ese orden."
else
  for modo in {0,1,2,3}
  do
    ./my_launch_files/hoGAZa/launch_calls_perf.sh MH01 $2 $modo ${1}_modo${modo}
  done
fi
