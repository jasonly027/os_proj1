#pragma once

#include <iostream>
#include <ostream>
#include <random>

namespace proj1 {

class JobGen {
   public:
    static constexpr int kDefaultMinBurstTime = 1;
    static constexpr int kDefaultMaxBurstTime = kDefaultMinBurstTime + 39;

    explicit JobGen(int a, int b, std::ostream& oss = std::cout);

    explicit JobGen(std::ostream& oss = std::cout);

    void operator()(int n);

    const std::ostream& oss() const;
    std::ostream& oss();
    void oss(std::ostream& oss);

   private:
    std::uniform_int_distribution<> rand_;
    std::mt19937 gen_;
    std::reference_wrapper<std::ostream> oss_;
};

}  // namespace proj1
