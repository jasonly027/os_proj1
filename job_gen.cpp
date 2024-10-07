#include <cstdlib>
#include <iostream>
#include <random>

using std::cout, std::cerr, std::atoi, std::exit;

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

    const int max_burst_time =
        (argc == 3) ? atoi(argv[2]) : kDefaultMaxBurstTime;
    if (max_burst_time <= 0) {
        cerr << "Invalid max_burst_time\n";
        exit(1);
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> rand(1, max_burst_time);
    for (int job_num = 1; job_num <= num_jobs; ++job_num) {
        cout << "Job" << job_num << '\n' << rand(gen) << '\n';
    }
}
