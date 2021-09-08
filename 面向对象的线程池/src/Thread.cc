#include "Thread.hpp"

Thread::Thread()
    : _pthid(0), _isRunning(false) //一开始线程还没有运行
{
}

Thread::~Thread()
{
    if (_isRunning)
    {
        pthread_detach(_pthid); //线程控制权交给系统
        _isRunning = false;
    }
}

void Thread::start()
{
    pthread_create(&_pthid, nullptr, threadFunc, this);
    _isRunning = true;
}

void *Thread::threadFunc(void *arg)
{
    Thread *pthread = static_cast<Thread *>(arg);
    if (pthread)
    {
        pthread->run();
    }
    return nullptr;
}

void Thread::join()
{
    if (_isRunning)
    {
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}
