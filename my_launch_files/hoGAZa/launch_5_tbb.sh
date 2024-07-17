#!/bin/bash
if [ $# -lt 1 ]; then
  echo "Debes pasar el nombre base de la carpeta de resultados"
else
  ./my_launch_files/hoGAZa/launch_single_call_v3_tbb.sh MH01 5 $1
fi
