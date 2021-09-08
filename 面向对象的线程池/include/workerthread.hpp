#ifndef __WORKERTHREAD_HPP__
#define __EORKERTHREAD_HPP__

#include "Thread.hpp"
#include "threadpool.hpp"

class WorkerThread
: public Thread
{
public:
    WorkerThread(ThreadPool &threadpool) 
    : _threadpool(threadpool)
    {}
private:
    void run()
    {
        _threadpool.workerTask();
    }

private:
    ThreadPool &_threadpool;
};

#endif