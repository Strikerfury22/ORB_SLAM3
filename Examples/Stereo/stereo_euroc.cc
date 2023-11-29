/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2021 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<chrono>

#include <tbb/tbb.h>

#include<opencv2/core/core.hpp>

#include<System.h>
#include"tbb_utils.hpp"
#include "pipeline_timer.hpp"

#define ROULETTE_TOKENS_FACTOR 1

using namespace std;

void LoadImages(const string &strPathLeft, const string &strPathRight, const string &strPathTimes,
                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimeStamps);

int main(int argc, char **argv)
{  
    auto t = std::chrono::high_resolution_clock::now();
    warmupTBB();

    std::cout << "START\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
    
    if(argc < 7)
    {
        cerr << endl << "Usage: ./stereo_euroc tokens_pipeline pf_grainsize path_to_vocabulary path_to_settings path_to_sequence_folder_1 path_to_times_file_1 (path_to_image_folder_2 path_to_times_file_2 ... path_to_image_folder_N path_to_times_file_N) (trajectory_file_name)" << endl;

        return 1;
    }

    const int num_tokens_pipeline = atoi(argv[1]);
    const int roulette_size = ROULETTE_TOKENS_FACTOR * num_tokens_pipeline;
    const int grainsize = atoi(argv[2]);
    const int num_seq = (argc-5)/2;
    cout << "num_seq = " << num_seq << endl;
    bool bFileName= (((argc-5) % 2) == 1);
    string file_name;
    if (bFileName)
    {
        file_name = string(argv[argc-1]);
        cout << "file name: " << file_name << endl;
    }

    // Load all sequences:
    int seq;
    vector< vector<string> > vstrImageLeft;
    vector< vector<string> > vstrImageRight;
    vector< vector<double> > vTimestampsCam;
    vector<int> nImages;

    vstrImageLeft.resize(num_seq);
    vstrImageRight.resize(num_seq);
    vTimestampsCam.resize(num_seq);
    nImages.resize(num_seq);

    //Arrays for image (roulette)
    cv::Mat *imgsLeft = new cv::Mat[roulette_size];
    cv::Mat *imgsRight = new cv::Mat[roulette_size];
    ORB_SLAM3::Frame *frames = new ORB_SLAM3::Frame[roulette_size];
    ORB_SLAM3::ORBextractor **extractorsLeft = new ORB_SLAM3::ORBextractor*[roulette_size];
    ORB_SLAM3::ORBextractor **extractorsRight = new ORB_SLAM3::ORBextractor*[roulette_size];
    #ifdef REGISTER_TIMES
        double *times_load = new double[roulette_size]; //I need to keep it here to insert it in the sequential Track stage so they are in order
    #endif
    int tot_images = 0;
    for (seq = 0; seq<num_seq; seq++)
    {
        cout << "Loading images for sequence " << seq << "...";

        string pathSeq(argv[(2*seq) + 5]);
        string pathTimeStamps(argv[(2*seq) + 6]);

        string pathCam0 = pathSeq + "/mav0/cam0/data";
        string pathCam1 = pathSeq + "/mav0/cam1/data";

        LoadImages(pathCam0, pathCam1, pathTimeStamps, vstrImageLeft[seq], vstrImageRight[seq], vTimestampsCam[seq]);
        cout << "LOADED!" << endl;

        nImages[seq] = vstrImageLeft[seq].size();
        tot_images += nImages[seq];
    }

    // Vector for tracking time statistics
    vector<double> vTimesTrack;
    vTimesTrack.resize(tot_images);

    cout << endl << "-------" << endl;
    cout.precision(17);

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(argv[3],argv[4],ORB_SLAM3::System::STEREO, false, grainsize);

    //Initialize ORBextractors
    int nFeatures = SLAM.settings_->nFeatures();
    int nLevels = SLAM.settings_->nLevels();
    int fIniThFAST = SLAM.settings_->initThFAST();
    int fMinThFAST = SLAM.settings_->minThFAST();
    float fScaleFactor = SLAM.settings_->scaleFactor();
    for(int i=0; i<roulette_size; i++){
        extractorsLeft[i] = new ORB_SLAM3::ORBextractor(nFeatures,fScaleFactor,nLevels,fIniThFAST,fMinThFAST);
        extractorsRight[i] = new ORB_SLAM3::ORBextractor(nFeatures,fScaleFactor,nLevels,fIniThFAST,fMinThFAST);
    }



    PipelineTimer ptimer(nImages[0], 3);

    cv::Mat imLeft, imRight;
    for (seq = 0; seq<num_seq; seq++)
    {
        t = std::chrono::high_resolution_clock::now();
        std::cout << "ALGO_START\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
        // Seq loop
        double t_resize = 0;
        double t_rect = 0;
        double t_track = 0;
        int num_rect = 0;
        int proccIm = 0;

        //nImages[seq] = 15; //TOP IMAGES FOR DEBUGGING

        int n_image = 0;
        

        tbb::parallel_pipeline(num_tokens_pipeline,
            //Dummy stage to stablish the order of the frames for the parallel stages
            tbb::make_filter<void, int>(tbb::filter_mode::serial_in_order,
            [&n_image, seq, &nImages](tbb::flow_control& fc) { 
                if( n_image == nImages[seq] ) {
                    fc.stop();
                    return -1;
                }

                return n_image++;
            }) & 
            // Read left and right images from file
            tbb::make_filter<int, int>(tbb::filter_mode::parallel,
            [&SLAM, &vstrImageLeft, &vstrImageRight, &imgsLeft, &imgsRight, seq, &ptimer, &vTimesTrack, &times_load, &roulette_size](int n_image) {
                ptimer.start_pipeline(n_image, 0);


                std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

                
                cv::Mat imLeft = cv::imread(vstrImageLeft[seq][n_image],cv::IMREAD_UNCHANGED); //,cv::IMREAD_UNCHANGED);
                cv::Mat imRight = cv::imread(vstrImageRight[seq][n_image],cv::IMREAD_UNCHANGED); //,cv::IMREAD_UNCHANGED);

                if(imLeft.empty())
                {
                    cerr << endl << "Failed to load image at: "
                        << string(vstrImageLeft[seq][n_image]) << endl;
                    exit(1);
                }

                if(imRight.empty())
                {
                    cerr << endl << "Failed to load image at: "
                        << string(vstrImageRight[seq][n_image]) << endl;
                    exit(1);
                }

                imgsLeft[n_image % roulette_size] = imLeft;
                imgsRight[n_image % roulette_size] = imRight;


                std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                double t_load = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
                vTimesTrack[n_image] = t_load;

                #ifdef REGISTER_TIMES
                    times_load[n_image % roulette_size] = t_load;
                #endif

                ptimer.end_pipeline(n_image, 0);
                return n_image;
            }) &
            //Create Frame from image
            tbb::make_filter<int, int>(tbb::filter_mode::parallel,
            [&SLAM, &frames, &ptimer, &seq, &vTimestampsCam, &vstrImageLeft, &imgsLeft, &imgsRight, &vTimesTrack, &extractorsLeft, &extractorsRight, &roulette_size](int n_image) {
                ptimer.start_pipeline(n_image, 1);

                std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

                frames[n_image % roulette_size] = SLAM.GenerateFrame(n_image, imgsLeft[n_image % roulette_size], 
                    imgsRight[n_image % roulette_size], extractorsLeft[n_image % roulette_size], extractorsRight[n_image % roulette_size],
                    vTimestampsCam[seq][n_image], vector<ORB_SLAM3::IMU::Point>(), vstrImageLeft[seq][n_image]);


                std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                double t_extract = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
                vTimesTrack[n_image] += t_extract;
                    
                ptimer.end_pipeline(n_image, 1);
                return n_image;
            }) &
            // Last stage ORB
            tbb::make_filter<int, void>(tbb::filter_mode::serial_in_order,
            [&SLAM, &vTimesTrack, &frames, seq, &ptimer, &vTimesTrack, &times_load, &roulette_size](int n_image) {
                ptimer.start_pipeline(n_image, 2);


                std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();


                SLAM.TrackFrame(frames[n_image % roulette_size]);

                std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                double t_track = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
                vTimesTrack[n_image] += t_track;
                double ttrack = vTimesTrack[n_image]; //Doesn't work????
                    
            #ifdef REGISTER_TIMES
                    SLAM.InsertLoadTime(times_load[n_image % roulette_size]);
                    SLAM.InsertTrackTime(ttrack);
                    SLAM.InsertVoidFrameTime();
            #endif


                ptimer.end_pipeline(n_image, 2);
            })); //END OF PIPELINE

        if(seq < num_seq - 1)
        {
            cout << "Changing the dataset" << endl;

            SLAM.ChangeDataset();
        }
        t = std::chrono::high_resolution_clock::now();
        std::cout << "ALGO_END\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
    }
    // Stop all threads
    SLAM.Shutdown();
    t = std::chrono::high_resolution_clock::now();

    ptimer.printStageTimesToFile(); //Print outside of sequence. If sequences are used, should use several ptimers on a vector.

    // Save camera trajectory
    if (bFileName)
    {
        const string kf_file = string(argv[argc-1]) + "_kf.txt";
        const string f_file = string(argv[argc-1]) + "_f.txt";
        SLAM.SaveTrajectoryEuRoC(f_file);
        SLAM.SaveKeyFrameTrajectoryEuRoC(kf_file);
    }
    else
    {
        SLAM.SaveTrajectoryEuRoC("CameraTrajectory.txt");
        SLAM.SaveKeyFrameTrajectoryEuRoC("KeyFrameTrajectory.txt");
    }
    
    std::cout << "END\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
    return 0;
}

void LoadImages(const string &strPathLeft, const string &strPathRight, const string &strPathTimes,
                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimeStamps)
{
    ifstream fTimes;
    fTimes.open(strPathTimes.c_str());
    vTimeStamps.reserve(5000);
    vstrImageLeft.reserve(5000);
    vstrImageRight.reserve(5000);
    while(!fTimes.eof())
    {
        string s;
        getline(fTimes,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            vstrImageLeft.push_back(strPathLeft + "/" + ss.str() + ".png");
            vstrImageRight.push_back(strPathRight + "/" + ss.str() + ".png");
            double t;
            ss >> t;
            vTimeStamps.push_back(t/1e9);

        }
    }
}
