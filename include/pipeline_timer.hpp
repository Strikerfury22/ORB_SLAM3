#include <chrono>
#include <vector>
#include <fstream>

class PipelineTimer {
public:
    PipelineTimer(int num_items, int num_stages);

    void start_pipeline(int num_item, int num_stage);
    void end_pipeline(int num_item, int num_stage);
    void printStageTimesToFile(std::string filename = "");

private:
    std::chrono::high_resolution_clock::time_point _ref_time;
    std::vector<std::vector<std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>>> _stage_times;
};