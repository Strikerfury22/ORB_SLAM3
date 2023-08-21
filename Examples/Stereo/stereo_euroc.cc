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

//TODO: Move to an appropiate include file
#define TOKENS_PIPELINE 10 
//The number of tokens divided by the number of stages is how many images will be on the fly because the last stage will always be sequential
#define ROULETTE_SIZE 10


using namespace std;

void LoadImages(const string &strPathLeft, const string &strPathRight, const string &strPathTimes,
                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimeStamps);

int main(int argc, char **argv)
{  
    auto t = std::chrono::high_resolution_clock::now();
    warmupTBB();

    std::cout << "START\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
    
    if(argc < 5)
    {
        cerr << endl << "Usage: ./stereo_euroc path_to_vocabulary path_to_settings path_to_sequence_folder_1 path_to_times_file_1 (path_to_image_folder_2 path_to_times_file_2 ... path_to_image_folder_N path_to_times_file_N) (trajectory_file_name)" << endl;

        return 1;
    }

    const int num_seq = (argc-3)/2;
    cout << "num_seq = " << num_seq << endl;
    bool bFileName= (((argc-3) % 2) == 1);
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
    cv::Mat imgsLeft[ROULETTE_SIZE];
    cv::Mat imgsRight[ROULETTE_SIZE];

    int tot_images = 0;
    for (seq = 0; seq<num_seq; seq++)
    {
        cout << "Loading images for sequence " << seq << "...";

        string pathSeq(argv[(2*seq) + 3]);
        string pathTimeStamps(argv[(2*seq) + 4]);

        string pathCam0 = pathSeq + "/mav0/cam0/data";
        string pathCam1 = pathSeq + "/mav0/cam1/data";

        LoadImages(pathCam0, pathCam1, pathTimeStamps, vstrImageLeft[seq], vstrImageRight[seq], vTimestampsCam[seq]);
        cout << "LOADED!" << endl;

        nImages[seq] = vstrImageLeft[seq].size();
        tot_images += nImages[seq];
    }

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(tot_images);

    cout << endl << "-------" << endl;
    cout.precision(17);

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::STEREO, false);

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

        nImages[seq] = 15; //TOP IMAGES FOR DEBUGGING

        int n_image = 0;
        PipelineTimer ptimer(nImages[seq], 2);

        tbb::parallel_pipeline(TOKENS_PIPELINE,
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
            [&vstrImageLeft, &vstrImageRight, &imgsLeft, &imgsRight, seq, &ptimer](int n_image) {
                ptimer.start_pipeline(n_image, 0);
                
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

                imgsLeft[n_image % ROULETTE_SIZE] = imLeft;
                imgsRight[n_image % ROULETTE_SIZE] = imRight;

                ptimer.end_pipeline(n_image, 0);
                return n_image;
            }) &
            // Last stage ORB
            tbb::make_filter<int, void>(tbb::filter_mode::serial_in_order,
            [&SLAM, &vTimestampsCam, &vstrImageLeft, &vTimesTrack, &imgsLeft, &imgsRight, seq, &ptimer](int n_image) {
                ptimer.start_pipeline(n_image, 1);

                double tframe = vTimestampsCam[seq][n_image];

                #ifdef COMPILEDWITHC11
                std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        #else
                std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
        #endif

                // Pass the images to the SLAM system
                SLAM.TrackStereo(imgsLeft[n_image % ROULETTE_SIZE], imgsRight[n_image % ROULETTE_SIZE],
                    tframe, vector<ORB_SLAM3::IMU::Point>(), vstrImageLeft[seq][n_image]);
                
        #ifdef COMPILEDWITHC11
                std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        #else
                std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
        #endif
                double t_track = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
    #ifdef REGISTER_TIMES
                SLAM.InsertTrackTime(t_track);
    #endif
                double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

                vTimesTrack[n_image]=ttrack;

                ptimer.end_pipeline(n_image, 1);
            })); //END OF PIPELINE

        if(seq < num_seq - 1)
        {
            cout << "Changing the dataset" << endl;

            SLAM.ChangeDataset();
        }
        t = std::chrono::high_resolution_clock::now();
        std::cout << "ALGO_END\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
        ptimer.printStageTimesToFile("test.dat");
    }
    // Stop all threads
    SLAM.Shutdown();

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

    t = std::chrono::high_resolution_clock::now();
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
