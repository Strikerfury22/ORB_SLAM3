#!/bin/bash
rm -r g2o/build
mkdir g2o/build
cd g2o/build
cmake -DCMAKE_INSTALL_PREFIX=/home/milarraza/orb_slam_libs ..
make -j 20
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/"
cd ../..
