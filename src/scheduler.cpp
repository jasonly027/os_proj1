#include "scheduler.h"

#include <fstream>
#include <iostream>

using std::string, std::ifstream;

namespace proj1 {
Job::Job(int n, int remaining_time)
    : n_(n), remaining_time_(remaining_time) {}

bool Job::do_for(int time) {
    remaining_time_ -= std::min(time, remaining_time_);
    return remaining_time_ == 0;
}

int Job::n() const { return n_; }

int Job::remaining_time() const { return remaining_time_; }

Log::Log(int job_n, int start, int end, bool done)
    : job_n(job_n), start(start), end(end), done(done) {}

void FCFS::load_impl(ifstream& ifs) {
    while (true) {
        string job_name;
        string burst_time;
        if (!(ifs >> job_name) || !(ifs >> burst_time)) return;

        const auto job = parse_job(job_name, burst_time);
        if (!job.has_value()) {
            std::cerr << "Job parse failed\n";
            return;
        }

        queue_.push_back(*job);
    }
}

int FCFS::run_impl() {
    int total_time = 0;
    while (!queue_.empty()) {
        auto job = queue_.front();
        queue_.pop_front();

        int time_taken = job.remaining_time();
        const bool done = job.do_for(time_taken);

        log(job.n(), time_taken, done);
    }

    return total_time;
}
}  // namespace proj1
