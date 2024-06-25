#!/bin/bash
rm -r opencv/build
mkdir -p opencv/build
cd opencv/build
cmake -DCMAKE_INSTALL_PREFIX=/home/milarraza/orb_slam_libs -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.9.0/modules ../opencv-4.9.0
make -j 20
make install
#cmake --install .. --prefix "/home/milarraza/orb_slam_libs/"
cd ../..
