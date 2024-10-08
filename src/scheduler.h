#pragma once

#include <deque>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace proj1 {
class Job {
   public:
    explicit Job(int n, int time);

    void do_for(int time);

    int n() const;
    int remaining_time() const;
    bool done() const;

   private:
    int n_;
    int remaining_time_;
    bool done_;
};

struct Log {
    int job_n;
    int start;
    int end;
    bool done;
};

template <typename CRTP>
class Scheduler {
   public:
    virtual ~Scheduler() = default;

    void load(std::ifstream& ifs) { static_cast<CRTP*>(this)->load_impl(ifs); }

    double run() { return static_cast<CRTP*>(this)->run_impl(); }

    void print_logs() const {
        for (const Log& log : logs_) std::cout << log.job_n << '\n';
    }

    static std::optional<Job> parse_job(const std::string& name_str,
                                        const std::string& time_str) {
        if (name_str.size() <= 3 || time_str.empty()) return {};

        try {
            const int n = std::stoi(name_str.substr(3));
            const int time = std::stoi(time_str);
            return Job{n, time};
        } catch (const std::invalid_argument& e) {
            return {};
        } catch (const std::out_of_range& e) {
            return {};
        }
    }

   protected:
    void log(const Job& job, int start, int end) {
        logs_.emplace_back(job.n(), start, end, job.done());
    }

    std::vector<Log> logs_;

   private:
    explicit Scheduler() = default;
    friend CRTP;
};

class FCFS : public Scheduler<FCFS> {
   public:
    void load_impl(std::ifstream& ifs);
    double run_impl();

   private:
    std::deque<Job> queue_;
};
}  // namespace proj1
