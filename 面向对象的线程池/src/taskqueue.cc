#include "taskqueue.hpp"

// #include <iostream>
// using std::cout;
// using std::endl;

TaskQueue::TaskQueue(size_t capacity)
: _capacity(capacity)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
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
void TaskQueue::push(const ElemType &elem)
{
    { //加这个语句块是为了精确的控制加锁范围
         AutoLock autoLock(_mutex);
        
        while(full())
        {
            _notFull.wait();
        }
        _que.push(elem);
    }

    _notEmpty.notify();

}

//运行消费者线程
ElemType TaskQueue::pop()
{
    AutoLock autolock(_mutex);

    while(_flag && empty()) {

		_notEmpty.wait();
        
	}
    if(_flag)
    {
        ElemType tmp = _que.front();
        _que.pop();

        _notFull.notify();

        return tmp;
    }
    else{
        return nullptr;
    }
    
    
}

void TaskQueue::weakup()
{
    _flag = false;
    _notEmpty.notifyall();
}
