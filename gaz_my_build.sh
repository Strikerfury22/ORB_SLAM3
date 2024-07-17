echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

echo "Configuring and building ORB_SLAM3 ..."

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/home/gaz/Desktop/MarcosIlarraza/orb_slam_libs;/home/gaz/Desktop/MarcosIlarraza/tbb_libs" ..
make
