#!/bin/bash
rm -r Pangolin/build
mkdir Pangolin/build
cd Pangolin/build
cmake .. #-DCMAKE_INSTALL_PREFIX=/home/gaz/Desktop/MarcosIlarraza/orb_slam_libs  ..
make
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/" # No chuta
cd ../..
