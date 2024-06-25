#!/bin/bash
rm -r DBoW2/build
mkdir DBoW2/build
cd DBoW2/build
cmake -DCMAKE_INSTALL_PREFIX=/home/milarrazza/orb_slam_libs -DCMAKE_PREFIX_PATH=/home/milarraza/orb_slam_libs ..
make -j 20
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/"
cd ../..
