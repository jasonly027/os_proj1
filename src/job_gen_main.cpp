#include <cstdlib>
#include <iostream>

#include "job_gen.h"

using std::cout, std::cerr, std::atoi, std::exit;

using proj1::JobGen;

int main(int argc, char** argv) {
    // Print guide if no arguments
    if (argc != 2 && argc != 3) {
        cout << "Usage: " << argv[0]
             << " num_jobs [max_burst_time=" << JobGen::kDefaultMaxBurstTime
             << "]\n";
        exit(0);
    }

    // Extract number of jobs
    const int num_jobs = atoi(argv[1]);
    if (num_jobs <= 0) {
        cerr << "Invalid num_jobs\n";
        exit(1);
    }

    JobGen gen = [&]() {
        // If user provides max_burst_time argument, use it
        if (argc == 3) {
            // Extract max burst time of a job
            const int max_burst_time = atoi(argv[2]);
            if (max_burst_time <= 0) {
                cerr << "Invalid max_burst_time\n";
                exit(1);
            }
            return JobGen(1, max_burst_time);
        }
        return JobGen();
    }();

    gen(num_jobs);
}
