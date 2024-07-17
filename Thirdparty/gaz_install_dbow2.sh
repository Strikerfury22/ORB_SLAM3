#!/bin/bash
rm -r DBoW2/build
mkdir DBoW2/build
cd DBoW2/build
cmake -DCMAKE_INSTALL_PREFIX=/home/gaz/Desktop/MarcosIlarraza/orb_slam_libs -DCMAKE_PREFIX_PATH=/home/gaz/Desktop/MarcosIlarraza/orb_slam_libs ..
make
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/"
cd ../..
