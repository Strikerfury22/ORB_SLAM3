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
#include "Optimizer.h"
#include"tbb_utils.hpp"
#include "pipeline_timer.hpp"

#define ROULETTE_TOKENS_FACTOR 1

using namespace std;

#ifdef REGISTER_MMUTEXMAPUPDATE


void guardarMMutexMapUpdateEnFichero(ORB_SLAM3::System &slamSystem){
    std::cout << "Guardando en formato JSON" << std::endl;
    //Buscamos el tiempo de inicio (para asignarlo como tiempo cero)
    std::cout << "ListaDatos1" << std::endl;
    double tiempo0 = slamSystem.getTracker()->getMetricasMMutexMapUpdate(0)[0];
    std::cout << "ListaDatos2" << std::endl;
    if (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(0).size() > 0 && tiempo0 > slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(0)[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(0)[0];
    }
    std::cout << "ListaDatos4" << std::endl;
    if (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(3).size() > 0 && tiempo0 > slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(3)[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(3)[0];
    }
    std::cout << "ListaDatos4" << std::endl;
    if (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(6).size() > 0 && tiempo0 > slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(6)[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(6)[0];
    }
    std::cout << "ListaDatos5" << std::endl;
    if (ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LM.size() > 0 && tiempo0 > ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LM[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LM[0];
    }
    std::cout << "ListaDatos6" << std::endl;
    if (ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal.size() > 0 && tiempo0 > ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal[0];
    }
    std::cout << "ListaDatos7" << std::endl;
    if (ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal2.size() > 0 && tiempo0 > ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal2[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal2[0];
    }
    std::cout << "ListaDatos8" << std::endl;
    if (ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_loopClosing.size() > 0 && tiempo0 > ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_loopClosing[0]){
        std::cout << "Cambio" << std::endl;
        tiempo0 = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_loopClosing[0];
    }

    //Ya tenemos el tiempo cero. Empezamos la declaración de cada instancia que hace mutex lock.


    std::cout << "Intentando abrir fichero" << std::endl;

    std::ofstream outFile("ficheroSolicitudMutex.json");

    std::cout << "¿Abierto?" << std::endl;

    if (outFile.is_open()) {
        outFile << "{\"traceEvents\":[" << std::endl;
        //Declaración de los "procesos" de Chromium
        outFile << "{\"args\":{\"name\":\"TrackingAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":0,\"tid\":0,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"(LoopClosing)MergeLocalUpdateMapAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":1,\"tid\":1,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"(LoopClosing)MergeLocalCurrentMapAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":2,\"tid\":2,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"(LoopClosing)CorrectLoopAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":3,\"tid\":3,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"OptimizerLocalBundleAdjustmentCallAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":4,\"tid\":4,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"(LoopClosing)OptimizerLocalBundleAdjustmentMergeLocalCallAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":5,\"tid\":5,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"(LoopClosing)OptimizerEssentialGraphMergeLocalCallAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":6,\"tid\":6,\"ts\":0}," << std::endl;
        outFile << "{\"args\":{\"name\":\"(LoopClosing)OptimizerLocalBundleAdjustmentCorrectLoopCallAccess\"},\"cat\":\"__metadata\",\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":7,\"tid\":7,\"ts\":0}";

        //Por cada instancia, generamos sus gráficas
        int numElementos = slamSystem.getTracker()->getMetricasMMutexMapUpdate(0).size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = slamSystem.getTracker()->getMetricasMMutexMapUpdate(0)[i] - tiempo0;
            double duracion = (slamSystem.getTracker()->getMetricasMMutexMapUpdate(1)[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (slamSystem.getTracker()->getMetricasMMutexMapUpdate(1)[i] - tiempo0);
            duracion = (slamSystem.getTracker()->getMetricasMMutexMapUpdate(2)[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":" << ts << "}";
        }

        numElementos = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(0).size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(0)[i] - tiempo0;
            double duracion = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(1)[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":1,\"tid\":1,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(1)[i] - tiempo0);
            duracion = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(2)[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":1,\"tid\":1,\"ts\":" << ts << "}";
        }

        numElementos = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(3).size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(3)[i] - tiempo0;
            double duracion = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(4)[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":2,\"tid\":2,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(4)[i] - tiempo0);
            duracion = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(5)[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":2,\"tid\":2,\"ts\":" << ts << "}";
        }

        numElementos = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(6).size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(6)[i] - tiempo0;
            double duracion = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(7)[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":3,\"tid\":3,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(7)[i] - tiempo0);
            duracion = (slamSystem.getLoopCloser()->getMetricasMMutexMapUpdate(8)[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":3,\"tid\":3,\"ts\":" << ts << "}";
        }

        numElementos = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LM.size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LM[i] - tiempo0;
            double duracion = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LM[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":4,\"tid\":4,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LM[i] - tiempo0);
            duracion = (ORB_SLAM3::Optimizer::listaLiberaciones_mMutexMapUpdate_LM[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":4,\"tid\":4,\"ts\":" << ts << "}";
        }

        numElementos = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal.size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal[i] - tiempo0;
            double duracion = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LC_mergeLocal[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":5,\"tid\":5,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LC_mergeLocal[i] - tiempo0);
            duracion = (ORB_SLAM3::Optimizer::listaLiberaciones_mMutexMapUpdate_LC_mergeLocal[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":5,\"tid\":5,\"ts\":" << ts << "}";
        }

        numElementos = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal2.size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_mergeLocal2[i] - tiempo0;
            double duracion = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LC_mergeLocal2[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":6,\"tid\":6,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LC_mergeLocal2[i] - tiempo0);
            duracion = (ORB_SLAM3::Optimizer::listaLiberaciones_mMutexMapUpdate_LC_mergeLocal2[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":6,\"tid\":6,\"ts\":" << ts << "}";
        }

        numElementos = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_loopClosing.size();
        std::cout << "Primer Bucle" << std::endl;
        for (int i = 0; i < numElementos; i++){
            outFile << "," << std::endl;
            double ts = ORB_SLAM3::Optimizer::listaSolicitudes_mMutexMapUpdate_LC_loopClosing[i] - tiempo0;
            double duracion = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LC_loopClosing[i] - tiempo0) - ts;
            if (duracion > 0){
                outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"MutexRequest\",\"ph\":\"X\",\"pid\":7,\"tid\":7,\"ts\":" << ts << "}," << std::endl;
            }
            ts = (ORB_SLAM3::Optimizer::listaRecepciones_mMutexMapUpdate_LC_loopClosing[i] - tiempo0);
            duracion = (ORB_SLAM3::Optimizer::listaLiberaciones_mMutexMapUpdate_LC_loopClosing[i] - tiempo0) - ts;
            outFile << "{\"args\":{},\"cat\":\"sequence_manager\",\"dur\":" << duracion << ",\"name\":\"Executes\",\"ph\":\"X\",\"pid\":7,\"tid\":7,\"ts\":" << ts << "}";
        }

        //Ahora cerramos el fichero con los metadatos
        outFile << "\"metadata\":{}}";

        outFile.close();
    } else {
        std::cerr << "Unable to open the file: ficheroSolicitudMutex.json" << std::endl;
    }

    outFile.close();
    return;
}

#endif

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
    #else
        double *times_load = NULL;
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


    
    #ifdef REGISTER_TOTAL_LATENCY
        PipelineTimer ptimer(nImages[0], 1);
    #else
        PipelineTimer ptimer(nImages[0], 3);
    #endif

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
        
        std::chrono::steady_clock::time_point initCarga, finCarga;  //finCarga es el timestamp en el que la anterior imagen llegó de la cámara
                                                                    //initCarga es el timestamp en el que la imagen actual llegó de la cámara
        SLAM.getMPTracker()->setCurrentFramesSize(num_tokens_pipeline); //Inicializamos la estructura para varios mCurrentFrame                                       
        std::cout << "Inicia la pipeline" << std::endl;
        tbb::parallel_pipeline(num_tokens_pipeline,
            //Dummy stage to stablish the order of the frames for the parallel stages
            tbb::make_filter<void, int>(tbb::filter_mode::serial_in_order,
            [&n_image, seq, &nImages, &initCarga, &finCarga](tbb::flow_control& fc) { 
                /*//initCarga = std::chrono::steady_clock::now(); //Para la simulación de FPS de la cámara
                if (n_image > 0){  //Solo evita dormirse en el primer fotograma (como el baseline)
                    double milisegundos =  std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(initCarga-finCarga).count();
                    if (33 > milisegundos) usleep((33-milisegundos) * 1000);
                }*/
                if( n_image == nImages[seq] ) {
                    fc.stop();
                    return -1;
                }
                std::cout << "-----------------------------" << std::endl;
                std::cout << "Loaded image: " << n_image << "/" << nImages[seq]-1 << std::endl;
                std::cout << "-----------------------------" << std::endl;
                //finCarga = std::chrono::steady_clock::now(); //Para la simulación de FPS de la cámara
                return n_image++;
            }) & 
            // Read left and right images from file
            tbb::make_filter<int, int>(tbb::filter_mode::parallel,
            [&SLAM, &vstrImageLeft, &vstrImageRight, &imgsLeft, &imgsRight, seq, &ptimer, &vTimesTrack, &times_load, &roulette_size](int n_image) {
                ptimer.start_pipeline(n_image, 0);

                #ifdef MEDIR_TIEMPO_SECCIONES
                    #ifdef REGISTER_SECTION_LATENCY
                        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
                    #endif                 

                #endif
                
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

                #ifdef MEDIR_TIEMPO_SECCIONES
                    #ifdef REGISTER_SECTION_LATENCY
                        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                        double t_load = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
                        vTimesTrack[n_image] = t_load;
                    #endif

                #ifdef REGISTER_TIMES
                    #ifdef REGISTER_SECTION_LATENCY
                        times_load[n_image % roulette_size] = t_load;
                    #endif
                #endif
                #endif
                
                #ifndef REGISTER_TOTAL_LATENCY
                    ptimer.end_pipeline(n_image, 0);
                #endif
                return n_image;
            }) &
            //Create Frame from image
            tbb::make_filter<int, int>(tbb::filter_mode::parallel,
            [&SLAM, &frames, &ptimer, &seq, &vTimestampsCam, &vstrImageLeft, &imgsLeft, &imgsRight, &vTimesTrack, &extractorsLeft, &extractorsRight, &roulette_size](int n_image) {
                
                #ifndef REGISTER_TOTAL_LATENCY
                    ptimer.start_pipeline(n_image, 1);
                #endif
                
                #ifdef MEDIR_TIEMPO_SECCIONES
                    #ifdef REGISTER_SECTION_LATENCY
                        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
                    #endif
                #endif
                frames[n_image % roulette_size] = SLAM.GenerateFrame(n_image, imgsLeft[n_image % roulette_size], 
                    imgsRight[n_image % roulette_size], extractorsLeft[n_image % roulette_size], extractorsRight[n_image % roulette_size],
                    vTimestampsCam[seq][n_image], vector<ORB_SLAM3::IMU::Point>(), vstrImageLeft[seq][n_image]);

                #ifdef MEDIR_TIEMPO_SECCIONES
                    #ifdef REGISTER_SECTION_LATENCY
                        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                        double t_extract = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
                        vTimesTrack[n_image] += t_extract;
                    #endif
                #endif
                #ifndef REGISTER_TOTAL_LATENCY
                    ptimer.end_pipeline(n_image, 1);
                #endif
                return n_image;
            }) &
            // Last stage ORB
            #ifdef MAKE_LAST_STAGE_PARALLEL
            tbb::make_filter<int, void>(tbb::filter_mode::parallel,
            [&SLAM, &vTimesTrack, &frames, seq, &ptimer, &vTimesTrack, &times_load, &roulette_size](int n_image) {    
            #else
            tbb::make_filter<int, void>(tbb::filter_mode::serial_in_order,
            [&SLAM, &vTimesTrack, &frames, seq, &ptimer, &vTimesTrack, &times_load, &roulette_size](int n_image) {
            #endif
                int posicion = SLAM.getMPTracker()->assignSpaceCurrentFrames(frames[n_image % roulette_size]);
                if (posicion == -1){
                    std::cout << "Australopitecus" << std::endl;
                }
                #ifndef REGISTER_TOTAL_LATENCY
                    ptimer.start_pipeline(n_image, 2);
                #endif

                #ifdef MEDIR_TIEMPO_SECCIONES
                    #ifdef REGISTER_SECTION_LATENCY
                        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
                    #endif
                #endif
                std::cout << "Entra en TrackFrame." << std::endl;

                SLAM.TrackFrame(frames[n_image % roulette_size],posicion);
                
                std::cout << "Sale de TrackFrame." << std::endl;

                #ifdef MEDIR_TIEMPO_SECCIONES
                    #ifdef REGISTER_SECTION_LATENCY
                        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                        double t_track = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
                        vTimesTrack[n_image] += t_track;
                        double ttrack = vTimesTrack[n_image]; //Doesn't work????
                    #endif

                #ifdef REGISTER_TIMES
                        
                    #ifdef REGISTER_SECTION_LATENCY
                        SLAM.InsertTrackTime(ttrack);
                        SLAM.InsertLoadTime(times_load[n_image % roulette_size]);
                        SLAM.InsertVoidFrameTime();
                    #endif
                #endif
                #endif

                #ifndef REGISTER_TOTAL_LATENCY
                    ptimer.end_pipeline(n_image, 2);
                #else
                    ptimer.end_pipeline(n_image, 0);
                #endif
                std::cout << "Fin de la última etapa." << std::endl;
                SLAM.getMPTracker()->freeSpaceCurrentFrames(posicion);
            })); //END OF PIPELINE
            std::cout << "Acaba la pipeline" << std::endl;
        if(seq < num_seq - 1)
        {
            cout << "Changing the dataset" << endl;

            SLAM.ChangeDataset();
        }
        #ifdef MEDIR_TIEMPO_SECCIONES
        t = std::chrono::high_resolution_clock::now();
        std::cout << "ALGO_END\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
        #endif
    }
    
    #ifdef REGISTER_MMUTEXMAPUPDATE
        guardarMMutexMapUpdateEnFichero(SLAM);
    #endif
    std::cout << "Apagamos ORB-SLAM3" << std::endl;
    // Stop all threads
    SLAM.Shutdown();

    
    #ifdef MEDIR_TIEMPO_SECCIONES//
        t = std::chrono::high_resolution_clock::now();
        #ifndef REGISTER_TOTAL_LATENCY
            std::cout << "HORA DE GUARDAR EN EL FICHERO V1" << std::endl;
            ptimer.printStageTimesToFile(); //Print outside of sequence. If sequences are used, should use several ptimers on a vector.
        #else
            std::cout << "HORA DE GUARDAR EN EL FICHERO V2" << std::endl;
            ptimer.printStageTimesToFile("samplesPipeline.txt",true); //Print outside of sequence. If sequences are used, should use several ptimers on a vector.
        #endif
    #endif
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
    #ifdef MEDIR_TIEMPO_SECCIONES
        std::cout << "END\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() << std::endl;
    #endif
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
