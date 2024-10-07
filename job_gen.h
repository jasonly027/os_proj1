#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <ostream>
#include <random>

namespace proj1 {

class JobGen {
   public:
    static constexpr int kDefaultMinBurstTime = 1;
    static constexpr int kDefaultMaxBurstTime = 40;

    // Construct a job generator that outputs jobs with
    // burst times in the range of [a, b] to output stream oss
    explicit JobGen(int a = kDefaultMinBurstTime, int b = kDefaultMaxBurstTime,
                    std::ostream& oss = std::cout)
        : rand_(a, b), gen_(std::random_device{}()), oss_(oss) {
        assert(a <= b && "b must be >= than a");
    }

    explicit JobGen(std::ostream& oss)
        : JobGen(kDefaultMinBurstTime, kDefaultMaxBurstTime, oss) {}

    // output n jobs to output stream
    inline void operator()(int n) {
        for (int i = 1; i <= n; ++i)
            oss_.get() << "Job" << i << '\n' << rand_(gen_) << '\n';
    }

    // get/set output stream
    const std::ostream& oss() const { return oss_; }
    std::ostream& oss() { return oss_; }
    void oss(std::ostream& oss) { oss_ = oss; }

   private:
    std::uniform_int_distribution<> rand_;
    std::mt19937 gen_;
    std::reference_wrapper<std::ostream> oss_;
};

}  // namespace proj1
