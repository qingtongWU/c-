#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include "noncopyable.hpp"
#include <functional>

using ThreadCallback = std::function<void()>;

class Thread : Noncopyable
{
public:
    Thread(ThreadCallback &&cb);
    ~Thread();

    void start(); //开启子线程
    void join();  //接收子线程结束时候的返回值，并清理子线程资源

private:            
    static void *threadFunc(void *arg); //设为static防止this指针的干扰
private:
    pthread_t _pthid;
    bool _isRunning; //表示线程运行还是结束
    ThreadCallback _cb;
};
#endif