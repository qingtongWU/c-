#ifndef __TASKQUEUE_HPP__
#define __TASKQUEUE_HPP__

#include "mutex.hpp"
#include "condition.hpp"
#include <queue>

using std::queue;

class TaskQueue
{
public:
    TaskQueue(size_t capacity);//队列容量

    bool empty()const;
    bool full()const;
    void push(int num);
    int pop();

private:
    size_t _capacity;
    queue<int> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
};

#endif
