#include "job_gen.h"

#include <cassert>

namespace proj1 {

// Construct a job generator that outputs jobs with
// burst times in the range of [a, b] to output stream oss
JobGen::JobGen(int a, int b, std::ostream& oss)
    : rand_(a, b), gen_(std::random_device{}()), oss_(oss) {
    assert(a <= b && "b must be >= than a");
}

// Construct with default ranges
JobGen::JobGen(std::ostream& oss)
    : JobGen(kDefaultMinBurstTime, kDefaultMaxBurstTime, oss) {}

// output n jobs to output stream
void JobGen::operator()(int n) {
    for (int i = 1; i <= n; ++i)
        oss_.get() << "Job" << i << '\n' << rand_(gen_) << '\n';
}

// get/set output stream
const std::ostream& JobGen::oss() const { return oss_; }
std::ostream& JobGen::oss() { return oss_; }
void JobGen::oss(std::ostream& oss) { oss_ = oss; }

}  // namespace proj1
