echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

echo "Configuring and building ORB_SLAM3 ..."

rm -r build_times_tbb
mkdir build_times_tbb
cd build_times_tbb
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/home/gaz/Desktop/MarcosIlarraza/orb_slam_libs;/home/gaz/Desktop/MarcosIlarraza/tbb_libs" -DDIRECTORIO_LIB="lib_tbb_times" ..
make
cd ..
