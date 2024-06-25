#!/bin/bash
rm -r Sophus/build
mkdir Sophus/build
cd Sophus/build
cmake -DCMAKE_INSTALL_PREFIX=/home/milarraza/orb_slam_libs ..
make -j 2
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/"
cd ../..
