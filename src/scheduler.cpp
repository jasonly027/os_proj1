#include "scheduler.h"

#include <fstream>
#include <functional>
#include <queue>
#include <string>

using std::string, std::ifstream;

namespace proj1 {

// Construct a Job with job number n and burst time time
Job::Job(int n, int time) : n_(n), remaining_time_(time), done_(false) {}

// Work on this job for the set time. Set done_ to true
// when the job is finished. Return time used.
int Job::do_for(int time) {
    int time_used = std::min(time, remaining_time_);
    remaining_time_ -= time_used;

    if (remaining_time_ == 0) done_ = true;
    return time_used;
}

// member getters
int Job::n() const { return n_; }
int Job::remaining_time() const { return remaining_time_; }
bool Job::done() const { return done_; }

Log::Log(int n, int start, int end, bool done)
    : job_n(n), start(start), end(end), done(done) {}

void FCFS::push_impl(int name, int time) { queue_.emplace_back(name, time); }

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

        elapsed += job.do_for(job.remaining_time());

        // Log job#, start/end time, if job was completed
        log(job, start, elapsed);
        total_time += elapsed;
    }

    // Return average turnaround time
    return total_time / num_jobs;
}

SJF::SJF()
    : heap_([](Job a, Job b) {
          // Set comparater so it's a min heap based off burst time
          return a.remaining_time() > b.remaining_time();
      }) {}

void SJF::push_impl(int name, int time) { heap_.emplace(name, time); }

double SJF::run_impl() {
    int elapsed = 0;

    double total_time = 0;
    const int num_jobs = heap_.size();

    // Pop shortest jobs from min heap and do them
    // till completion.
    while (!heap_.empty()) {
        Job job = heap_.top();
        heap_.pop();

        const int start = elapsed;

        elapsed += job.do_for(job.remaining_time());

        // Log job#, start/end time, if job was completed
        log(job, start, elapsed);
        total_time += elapsed;
    }

    // Return average turnaround time
    return total_time / num_jobs;
}

}  // namespace proj1
