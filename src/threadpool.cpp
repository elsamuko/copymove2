#include <cassert>
#include <chrono>

#include "threadpool.hpp"

// heavily influenced by
// https://github.com/progschj/ThreadPool

ThreadPool::ThreadPool() {
    mRunning.store( true );
    mJobCount.store( 0 );

    size_t count = std::thread::hardware_concurrency();
    assert( count > 0 );

    while( count-- ) {
        mWorkers.emplace_back( [this] {
            for( ;; ) {

                if( !jobsAvailable() ) {
                    this->standInLine();
                }

                this->workOff();

                if( !( mRunning.load() || mJobCount.load() ) ) {
                    break;
                }

            }
        } );
    }
}

ThreadPool::~ThreadPool() {
    mRunning.store( false );
    waitForAllJobs();

    for( std::thread & worker : mWorkers ) {
        worker.join();
    }
}

void ThreadPool::workOff() {
    Job job;
    {
        std::unique_lock<std::mutex> lock( mJobMutex );

        if( !mJobs.empty() ) {
            job = mJobs.front();
            mJobs.pop();
        }
    }

    if( job ) {
        job();
        // decrement _after_ job is done
        mJobCount--;
    }
}

void ThreadPool::standInLine() {
    std::unique_lock<std::mutex> lock( mWaitingMutex );
    mWaitingCondition.wait( lock );
}

bool ThreadPool::jobsAvailable() {
    return mJobCount.load() > 0;
}

void ThreadPool::add( const Job& job ) {
    assert( mRunning.load() );
    std::unique_lock<std::mutex> lock( mJobMutex );
    mJobs.emplace( job );
    mJobCount++;
    mWaitingCondition.notify_one();
}

void ThreadPool::waitForAllJobs() {
    mWaitingCondition.notify_all();

    while( mJobCount.load() > 0 ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        mWaitingCondition.notify_all();
    }
}

int ThreadPool::size() const {
    return mWorkers.size();
}
