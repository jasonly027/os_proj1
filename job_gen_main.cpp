#include <cstdlib>
#include <iostream>

#include "job_gen.h"

using std::cout, std::cerr, std::atoi, std::exit;

using proj1::JobGen;

static constexpr int kDefaultMaxBurstTime = 40;

int main(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
        cout << "Usage: " << argv[0]
             << " num_jobs [max_burst_time=" << kDefaultMaxBurstTime << "]\n";
        exit(0);
    }

    const int num_jobs = atoi(argv[1]);
    if (num_jobs <= 0) {
        cerr << "Invalid num_jobs\n";
        exit(1);
    }

    JobGen gen = [&]() {
        if (argc == 3)
            if (const int max_burst_time = atoi(argv[2]); max_burst_time > 0)
                return JobGen(1, max_burst_time);
        return JobGen();
    }();

    gen(num_jobs);
}
