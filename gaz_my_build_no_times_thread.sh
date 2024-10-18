echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

echo "Configuring and building ORB_SLAM3 ..."

rm -r build_no_times_thread
mkdir build_no_times_thread
cd build_no_times_thread
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/home/gaz/Desktop/MarcosIlarraza/orb_slam_libs;/home/gaz/Desktop/MarcosIlarraza/tbb_libs" -DDIRECTORIO_LIB="lib_thread_no_times" ..
make
cd..
