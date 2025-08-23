#include "CFPSFix.h"
#include "main.h"
#include <sys/syscall.h>
#include <dirent.h>
#include <filesystem>

constexpr uint32_t CPU_AFFINITY_MASK = 255;

CFPSFix::CFPSFix() {
    auto routineThread = std::make_shared<std::thread>(&CFPSFix::Routine, this);
    routineThread->detach();
}

void CFPSFix::Routine() {
    while (true) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);

        try {
            for (const auto &entry: std::filesystem::directory_iterator("/proc/self/task")) {
                pid_t tid = std::stoi(entry.path().filename().string());
                if (syscall(__NR_sched_setaffinity, tid, sizeof(CPU_AFFINITY_MASK), &CPU_AFFINITY_MASK) == -1) {
                    DLOG("Failed to set affinity for TID");
                }
            }
        } catch (const std::exception &e) {
            DLOG("Routine: ", e.what());
        }
    }
}
