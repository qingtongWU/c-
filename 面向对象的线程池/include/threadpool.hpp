#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include "taskqueue.hpp"
#include "task.hpp"
#include <vector>
#include <memory>


using std::vector;
using std::unique_ptr;

class Thread;//类的前向声明

class ThreadPool
{
public:
    ThreadPool(size_t num,size_t capacity);

    void start();
    void stop();
    
    void addTask(Task *);

    ~ThreadPool();

    friend class WorkerThread;//友元类
private:
    void workerTask();
    Task *getTask();

private:
    vector<unique_ptr<Thread>> _threads;
    size_t _num;//线程数量
    size_t _capacity;
    TaskQueue _taskque;
    bool _exit;//线程退出标记

};

#endif