### noncopyable.hpp

~~~c
#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__

class Noncopyable{
protected:
    Noncopyable() {}
    ~Noncopyable() {}
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};
#endif
~~~

### Thread.hpp

~~~c
#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include "noncopyable.hpp"
#include <pthread.h>
#include <functional>

using ThreadCallback = std::function<void()>;

class Thread
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
~~~

### Thread.cc

~~~c
#include "Thread.hpp"

Thread::Thread(ThreadCallback &&cb)
    : _pthid(0)
    , _isRunning(false)
    , _cb(std::move(cb)) //转为右值
    { }

Thread::~Thread(){
    if (_isRunning){
        pthread_detach(_pthid); //线程控制权交给系统
        _isRunning = false;
    }
}

void Thread::start(){
    pthread_create(&_pthid, nullptr, threadFunc, this);
    _isRunning = true;
}

void *Thread::threadFunc(void *arg){
    Thread *pthread = static_cast<Thread *>(arg);
    if (pthread){
        pthread->_cb();
    }
    return nullptr;
}

void Thread::join(){
    if (_isRunning){
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}
~~~

### test.cc

~~~c
#include "Thread.hpp"
#include <unistd.h>
#include <memory>
#include <iostream>

using std::cout;
using std::endl;
using std::unique_ptr;

class Task{
public:
    void run(){
        int count = 10;
        srand(time(nullptr)); //生成随机数种子

        while (count > 0){
            int num = rand() % 100; //生成100以内的随机数
            cout << "num = " << num << endl;
            count--;
            sleep(1);
        }
    }
};

void test(){
    Task task;
    ThreadCallback tck = std::bind(&Task::run,&task);
    unique_ptr<Thread> thread(new Thread(std::move(tck)));

    thread->start();
    thread->join();
}

int main(void){
    test();
    return 0;
}
~~~

