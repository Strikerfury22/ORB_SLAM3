#include "pipeline_timer.hpp"
#include <iostream>

PipelineTimer::PipelineTimer(int num_items, int num_stages)
:_stage_times(num_items, std::vector<std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>>(num_stages)) {
    _ref_time = std::chrono::high_resolution_clock::now();

    /*_stage_times.resize(num_items);
    for (int i = 0; i < num_items; ++i) {
        _stage_times[i].resize(num_stages);
    }*/
}

void PipelineTimer::start_pipeline(int num_item, int num_stage){
    _stage_times[num_item][num_stage].first = std::chrono::high_resolution_clock::now();
}

void PipelineTimer::end_pipeline(int num_item, int num_stage){
    _stage_times[num_item][num_stage].second = std::chrono::high_resolution_clock::now();
}

void PipelineTimer::printStageTimesToFile(std::string filename) {
    if(filename.empty()) {
        filename = std::string("PipelineTimer.dat");
    }
    std::ofstream outFile(filename);

    outFile << "Item\tStage\tTstart\tTend" << std::endl;

    if (outFile.is_open()) {
        for (int item = 0; item < _stage_times.size(); ++item) {
            for (int stage = 0; stage < _stage_times[item].size(); ++stage) {
                outFile << item << "\t" << stage << "\t"
                        << std::chrono::duration_cast<std::chrono::nanoseconds>(
                                _stage_times[item][stage].first - _ref_time).count() << "\t"
                        << std::chrono::duration_cast<std::chrono::nanoseconds>(
                                _stage_times[item][stage].second - _ref_time).count() << std::endl;
            }
        }
        outFile.close();
    } else {
        std::cerr << "Unable to open the file: " << filename << std::endl;
    }

    outFile.close();
}