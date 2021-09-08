#include "threadpool.hpp"
#include "workerthread.hpp"

#include <unistd.h>

ThreadPool::ThreadPool(size_t num,size_t capacity)
    : _num(num)
    , _capacity(capacity)
    , _taskque(_capacity)
    , _exit(false)
    {
        _threads.reserve(_num);//预先申请空间
    }

void ThreadPool::start()
{
    //创建子线程对象
    for(size_t idx = 0;idx < _num;++idx)
    {
        unique_ptr<Thread> uthread(new WorkerThread(*this));
        _threads.push_back(std::move(uthread));
    }
    //启动子线程
    for(auto &thread : _threads)
    {
        thread->start();
    }
}

//线程的退出
void ThreadPool::stop()
{
    if(!_exit)
    {
        while(!_taskque.empty())//所有线程都结束的时候才退出
        {
            sleep(1);
        }
        _exit = true;
        _taskque.weakup();

        for(auto &thread : _threads)
        {
            thread->join();//回收线程资源
        }
    }
}
    
void ThreadPool::addTask(Task *psk)
{
    if(psk)
    {
        _taskque.push(psk);
    }

}


ThreadPool::~ThreadPool()
{
    if(!_exit)
    {
        stop();
    }
}

//每一个工作线程都要调用这个函数
void ThreadPool::workerTask()
{
	while(!_exit) {
		Task * ptsk = getTask();
		if(ptsk) {
			ptsk->process();
		}
	}
}

Task *ThreadPool::getTask()
{
    return _taskque.pop();
}
