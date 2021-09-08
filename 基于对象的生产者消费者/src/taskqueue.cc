#include "taskqueue.hpp"

#include <iostream>
using std::cout;
using std::endl;

TaskQueue::TaskQueue(size_t capacity)
: _capacity(capacity)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
{}

bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _que.size() == _capacity;
}

//运行生产者线程
void TaskQueue::push(int num)
{
    {
         MutexLockGuard autoLock(_mutex);
        
        while(full())
        {
            _notFull.wait();
        }
        _que.push(num);
    }

    _notEmpty.notify();

}

//运行消费者线程
int TaskQueue::pop()
{
    MutexLockGuard autolock(_mutex);

    while(empty()) {

		_notEmpty.wait();//程序没有阻塞在这里
        
	}
    int tmp = _que.front();
    _que.pop();

    _notFull.notify();

    return tmp;
    
}
