#include <iostream>

#include "scheduler.h"

using proj1::FCFS, proj1::SJF, proj1::RR;
using std::cout;

template <typename Scheduler>
void run(const char* file) {
    Scheduler scheduler;
    cout << '\n' << typeid(scheduler).name() << '\n';

    scheduler.load(file);
    scheduler.print(scheduler.run());
}

int main() {
    static constexpr const char* kFiles[] = {"job5.txt", "job10.txt",
                                             "job15.txt"};

    for (const char* file : kFiles) {
        cout << "\n[Testing with " << file << "]\n";

        run<FCFS>(file);
        run<SJF>(file);
        run<RR<2>>(file);
    }
}
