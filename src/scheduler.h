#pragma once

#include <deque>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace proj1 {
class Job {
   public:
    explicit Job(int n, int remaining_time);

    bool do_for(int time);

    int n() const;
    int remaining_time() const;

   private:
    int n_;
    int remaining_time_;
};

struct Log {
    explicit Log(int job_n, int start, int end, bool done);

    int job_n;
    int start;
    int end;
    bool done;
};

template <typename CRTP>
class Scheduler {
   public:
    explicit Scheduler() = default;

    virtual ~Scheduler() = default;

    void load(const std::string& file_name) {
        std::ifstream ifs(file_name);
        if (!ifs.is_open()) {
            std::cerr << "Failed to load file\n";
            return;
        }

        static_cast<CRTP*>(this)->load_impl(ifs);
    }

    int run() { return static_cast<CRTP*>(this)->run_impl(); }

    void print_logs() const {
        for (const Log& log : logs_) std::cout << log.job_n << '\n';
    }

    static std::optional<Job> parse_job(const std::string& name_str,
                                        const std::string& time_str) {
        if (name_str.size() <= 3 || time_str.empty()) return {};

        const int n = std::stoi(name_str.substr(3));
        const int time = std::stoi(time_str);
        return Job{n, time};
    }

   protected:
    void log(int job_n, int time_taken, bool done) {
        static int now = 0;

        logs_.emplace_back(
            job_n,
            now,
            now += time_taken,
            done
        );
    }

    std::vector<Log> logs_;
};

class FCFS : public Scheduler<FCFS> {
   public:
    void load_impl(std::ifstream& ifs);
    int run_impl();

   private:
    std::deque<Job> queue_;
};
}  // namespace proj1
