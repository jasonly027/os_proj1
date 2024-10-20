#include <iostream>
#include <sstream>

#include "job_gen.h"
#include "scheduler.h"

using proj1::FCFS, proj1::SJF, proj1::RR, proj1::JobGen;
using std::cout, std::stringstream;

template <typename... Scheduler>
void run(const char* file) {
    cout << "\n[Testing with " << file << "]\n";

    // Expands to use every generic type Scheduler specified
    (([&] {
         Scheduler scheduler;
         cout << '\n' << Scheduler::name() << '\n';

         scheduler.load(file);
         scheduler.print(scheduler.run());
     }()),
     ...);
}

template <typename... Scheduler>
void analyze(int jobs) {
    cout << "\n" << jobs << " jobs\n\n";
    static constexpr int kTrials = 20;

    // Generate 20 datasets, one for each trial
    JobGen gen;
    stringstream streams[kTrials];
    for (auto& stream : streams) {
        gen.oss(stream);
        gen(jobs);
    }

    // Expands to use every generic type Scheduler specified
    (([&]() {
         double avg = 0;

         for (const stringstream& stream : streams) {
             Scheduler scheduler;
             // Makes a copy of the stream so other scheduler impls
             // can still use it
             scheduler.load(stringstream(stream.str()));

             avg += scheduler.run();
         }

         avg /= kTrials;

         cout << Scheduler::name() << ": " << avg << '\n';
     }()),
     ...);
}

int main() {
    // Testing Part 1b
    for (const char* file : {"job5.txt", "job10.txt", "job15.txt"})
        run<FCFS, SJF, RR<2>, RR<5>>(file);

    // Part 3 performance analysis
    for (const int jobs : {5, 10, 15}) analyze<FCFS, SJF, RR<2>, RR<5>>(jobs);
}
