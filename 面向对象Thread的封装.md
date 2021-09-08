### 设计理念

~~~c
1.为什么需要一个纯虚函数run?
为了得到一个通用的线程类，因为不知道线程要执行什么任务，任务本身时抽象的，只能用纯虚函数来表达。
2.ThreadFunc为啥要是一个静态的成员函数？
在实现start方法的时候，它需要调用pthread_create()来创建线程，该函数的第三个参数要求是一个返回值void*，参数也是void*的函数指针，threadFunc()时提供该功能的，为了消除this指针的影响，必须设置为静态的。
3.pthread_create的第四个参数为什么要传递this指针？
在threadFunc()内部要调用虚函数run方法，而run()方法的调用必须依赖Thread对象，要拿到线程对象，只能通过this指针。
4.run() & threadFunc()要设成私有的，为什么？
run()如果设计成public的，那可以直接在类外面通过Thread对象调用，而该方法代表的是要执行的任务，应该由子线程来完成调用，不能直接调用，同理，threadFunc()也是。
~~~

### Thread.hpp

~~~c
#ifndef __THREAD_HPP__
#define __THREAD_HPP__
#include <pthread.h>

class Thread{
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
~~~

### Thread.cc

~~~c
#include "Thread.hpp"

Thread::Thread()
: _pthid(0), _isRunning(false) //一开始线程还没有运行
{
}

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
        pthread->run();
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

class ChildThread
: public Thread
{
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
    unique_ptr<Thread> thread(new ChildThread());
    thread->start();
    thread->join();
}

int main(void){
    test();
    return 0;
}
~~~



