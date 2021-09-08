#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include "noncopyable.hpp"

class Thread : Noncopyable
{
public:
    Thread();
    virtual ~Thread();

    void start(); //开启子线程
    void join();  //接收子线程结束时候的返回值，并清理子线程资源

private:
    virtual void run() = 0;             //子线程要执行的任务
    static void *threadFunc(void *arg); //设为static防止this指针的干扰
private:
    pthread_t _pthid;
    bool _isRunning; //表示线程运行还是结束
};
#endif