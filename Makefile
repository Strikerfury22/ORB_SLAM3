ROOT_DIR=$(shell pwd)
THIRDPARTY_DIR=$(ROOT_DIR)/Thirdparty

${THIRDPARTY_DIR}/opencv :
	mkdir ${THIRDPARTY_DIR} -p
	rm ${THIRDPARTY_DIR}/opencv -rf
	git clone "https://github.com/opencv/opencv.git" ${THIRDPARTY_DIR}/opencv
	cd ${THIRDPARTY_DIR}/opencv && git checkout 4.4.0

opencv : ${THIRDPARTY_DIR}/opencv
	cd ${THIRDPARTY_DIR}/opencv && mkdir build_dir -p
	cd ${THIRDPARTY_DIR}/opencv && rm build_dir/* -rf
	cd ${THIRDPARTY_DIR}/opencv/build_dir && cmake -D CMAKE_BUILD_TYPE=RELWITHDEBINFO  -D CMAKE_INSTALL_PREFIX=$@    -DCMAKE_CXX_FLAGS="-Wno-error=address"             \
	-DENABLE_OMIT_FRAME_POINTER=OFF -DWITH_PTHREADS_PF=OFF -DWITH_GSTREAMER=OFF -DWITH_FFMPEG=OFF -DBUILD_PERF_TESTS=OFF  -D WITH_OPENCL=OFF  -D WITH_1394=OFF                \
	-D BUILD_TESTS=OFF  -D WITH_TBB=OFF  -D WITH_V4L=OFF  -D WITH_OPENGL=OFF -D BUILD_opencv_gpu=OFF    \
	  -D BUILD_opencv_java=OFF -D WITH_CUDA=OFF -DENABLE_PRECOMPILED_HEADERS=OFF .. > ${THIRDPARTY_DIR}/opencv/build_dir/opencv_cmake.log
	cat ${THIRDPARTY_DIR}/opencv/build_dir/opencv_cmake.log
	+cd ${THIRDPARTY_DIR}/opencv/build_dir && make
	mkdir -p $@
	cd ${THIRDPARTY_DIR}/opencv/build_dir && make install