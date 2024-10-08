#include "scheduler.h"

#include <fstream>
#include <iostream>

using std::string, std::ifstream, std::cerr;

namespace proj1 {

// Construct a Job with job number n and burst time time
Job::Job(int n, int time) : n_(n), remaining_time_(time), done_(false) {}

// Work on this job for a set time. Set done_ to true
// when the job is finished.
void Job::do_for(int time) {
    remaining_time_ -= std::min(time, remaining_time_);
    if (remaining_time_ == 0) done_ = true;
}

// member getters
int Job::n() const { return n_; }
int Job::remaining_time() const { return remaining_time_; }
bool Job::done() const { return done_; }

/////////////////////////////////////////
//                                     //
// Algorithm implemenations start here //
//                                     //
/////////////////////////////////////////

void FCFS::load_impl(ifstream& ifs) {
    // Parse jobs from stream and append them to the end of the queue
    while (true) {
        string job_name;
        string burst_time;
        if (!(ifs >> job_name)) return;
        if (!(ifs >> burst_time)) {
            cerr << "Job parse failed\n";
            return;
        }

        auto job = parse_job(job_name, burst_time);
        if (!job.has_value()) {
            cerr << "Job parse failed\n";
            return;
        }

        queue_.push_back(std::move(job.value()));
    }
}

double FCFS::run_impl() {
    int elapsed = 0;

    double total_time = 0;
    const int num_jobs = queue_.size();

    // Pop jobs from the front of the queue and do them
    // till completion.
    while (!queue_.empty()) {
        Job job = queue_.front();
        queue_.pop_front();

        const int start = elapsed;

        const int duration = job.remaining_time();
        job.do_for(duration);
        elapsed += duration;

        // At each turn, record the job number, start/end time,
        // and if the job was completed on this turn.
        log(job, start, elapsed);
        total_time += elapsed;
    }

    // Return average turnaround time
    return total_time / num_jobs;
}

}  // namespace proj1
