#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "log/log.hpp"

class ThreadPool {
    public:
        typedef std::function<void()> Job;
        ThreadPool();
        ~ThreadPool();
        void add( const ThreadPool::Job& job );
        void waitForAllJobs();
        int size() const;
    private:
        void workOff();
        void standInLine();
        bool jobsAvailable();

        std::vector<std::thread> mWorkers;

        std::mutex mWaitingMutex;
        std::condition_variable mWaitingCondition;

        mutable std::mutex mJobMutex;
        std::queue<Job> mJobs;
        std::atomic_int mJobCount;

        std::atomic_bool mRunning;
};

