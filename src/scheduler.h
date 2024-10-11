#pragma once

#include <cstdio>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <list>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace proj1 {
class Job {
   public:
    Job(int n, int time);

    int do_for(int time);

    int n() const;
    int remaining_time() const;
    bool done() const;

   private:
    int n_;
    int remaining_time_;
    bool done_;
};

struct Log {
    Log(int n, int start, int end, bool done);

    int job_n;
    int start;
    int end;
    bool done;
};

// Scheduler is defined in header file because it's a template class
template <typename CRTP>  // compile time polymorphism
class Scheduler {
   public:
    virtual ~Scheduler() = default;

    static std::string name() { return CRTP::name_impl(); }

    void load(const char* file) {
        std::ifstream ifs(file);
        if (!ifs.is_open()) {
            std::cerr << "Stream failed to open\n";
            return;
        }
        load(std::move(ifs));
    }

    void load(std::istream&& ifs) {
        auto* me = static_cast<CRTP*>(this);
        // Parse jobs from stream and push them to jobs container
        while (true) {
            std::string name_str;
            std::string time_str;
            // Return if no more jobs
            if (!(ifs >> name_str)) return;

            // Return if job has no accompanying burst time
            if (!(ifs >> time_str)) {
                std::cerr << "Job parse failed\n";
                return;
            }

            const auto opt = parse_name_time(name_str, time_str);
            // Return if parse failed
            if (!opt) {
                std::cerr << "Job parse failed\n";
                return;
            }
            const auto [name, time] = *opt;

            me->push_impl(name, time);
        }
    }

    double run() { return static_cast<CRTP*>(this)->run_impl(); }

    void print(double avg_turnaround) const {
        print_logs();
        std::cout << "Average turnaround was " << avg_turnaround << '\n';
    }

    void print_logs() const {
        // Print table header
        std::puts("\n| Job# | Start Time | End Time | Job Completion        |");

        // Print logs
        for (const Log& log : logs_) {
            const std::string done_str =
                (log.done) ? std::string("Job ") + std::to_string(log.job_n) +
                                 " completed @" + std::to_string(log.end)
                           : "";
            std::printf("| %-4d | %-10d | %-8d | %-21s |\n", log.job_n,
                        log.start, log.end, done_str.c_str());
        }

        std::puts("");
    }

    // Returns job number and burst time as ints if conversion is successful
    static std::optional<std::pair<int, int>> parse_name_time(
        const std::string& name_str, const std::string& time_str) {
        if (name_str.size() <= 3 || time_str.empty()) return {};

        try {
            const int n = std::stoi(name_str.substr(3));
            const int time = std::stoi(time_str);
            return std::make_pair(n, time);
        } catch (const std::invalid_argument& e) {
            return {};
        } catch (const std::out_of_range& e) {
            return {};
        }
    }

   protected:
    // Save to logs
    void log(const Job& job, int start, int end) {
        logs_.emplace_back(job.n(), start, end, job.done());
    }

    std::vector<Log> logs_;

   private:
    explicit Scheduler() = default;
    friend CRTP;
};

class FCFS : public Scheduler<FCFS> {
   private:
    constexpr static std::string name_impl() { return FCFS::kName; }
    void push_impl(int name, int time);
    double run_impl();

    friend Scheduler<FCFS>;
    std::deque<Job> queue_;
    static constexpr const char* kName = "FCFS";
};

class SJF : public Scheduler<SJF> {
   public:
    SJF();

   private:
    constexpr static std::string name_impl() { return SJF::kName; }
    void push_impl(int name, int time);
    double run_impl();

    friend Scheduler<SJF>;
    std::priority_queue<Job, std::vector<Job>,
                        std::function<bool(const Job&, const Job&)>>
        heap_;
    static constexpr const char* kName = "SJF";
};

// RR is defined in header file because it's a template class
template <int TimeSlice>
class RR : public Scheduler<RR<TimeSlice>> {
   private:
    static std::string name_impl() {
        return std::string("RR") + std::to_string(TimeSlice);
    }

    void push_impl(int name, int time) { list_.emplace_back(name, time); }

    double run_impl() {
        int elapsed = 0;

        double total_time = 0;
        const int num_jobs = list_.size();

        // Repeatedly iterate through the list (until its empty)
        // and work on jobs for a TimeSlice amount of time before
        // moving on to the next job.
        while (!list_.empty()) {
            auto job = list_.begin();
            while (job != list_.end()) {
                const int start = elapsed;

                elapsed += job->do_for(TimeSlice);

                // Log job#, start/end time, if job was completed
                this->log(*job, start, elapsed);

                // When a job is done, remove it from the list
                if (job->done()) {
                    total_time += elapsed;
                    job = list_.erase(job);
                    continue;
                }

                ++job;
            }
        }

        // Return average turnaround time
        return total_time / num_jobs;
    }

    friend Scheduler<RR<TimeSlice>>;
    std::list<Job> list_;
};
}  // namespace proj1
