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

./Datasets/EuRoCMAV/% :  # Example : $* = machine_hall/MH_01_easy/MH_01_easy
	cd $(EXTRA)
	mkdir -p $(@D)
	#cd $(@D)  &&  ${WGET} "http://robotics.ethz.ch/~asl-datasets/ijrr_euroc_mav_dataset/$*.zip"

EuRoCMAV :
	### EuRoCMAV Machine Hall ###"
	make ./Datasets/EuRoCMAV/machine_hall/MH_01_easy/MH_01_easy
	make ./Datasets/EuRoCMAV/machine_hall/MH_02_easy/MH_02_easy
	make ./Datasets/EuRoCMAV/machine_hall/MH_03_medium/MH_03_medium
	make ./Datasets/EuRoCMAV/machine_hall/MH_04_difficult/MH_04_difficult
	make ./Datasets/EuRoCMAV/machine_hall/MH_05_difficult/MH_05_difficult

	### EuRoCMAV Vicon Room ###
	make ./Datasets/EuRoCMAV/vicon_room1/V1_01_easy/V1_01_easy
	make ./Datasets/EuRoCMAV/vicon_room1/V1_02_medium/V1_02_medium
	make ./Datasets/EuRoCMAV/vicon_room1/V1_03_difficult/V1_03_difficult
	make ./Datasets/EuRoCMAV/vicon_room2/V2_01_easy/V2_01_easy
	make ./Datasets/EuRoCMAV/vicon_room2/V2_02_medium/V2_02_medium
	make ./Datasets/EuRoCMAV/vicon_room2/V2_03_difficult/V2_03_difficult
