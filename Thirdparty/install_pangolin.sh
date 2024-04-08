#!/bin/bash
rm -r Pangolin/build
mkdir Pangolin/build
cd Pangolin/build
cmake -DCMAKE_INSTALL_PREFIX=/home/milarraza/orb_slam_libs ..
make -j 20
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/" # No chuta
cd ../..
