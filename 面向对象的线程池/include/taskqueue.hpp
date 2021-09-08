#ifndef __TASKQUEUE_HPP__
#define __TASKQUEUE_HPP__

#include "mutex.hpp"
#include "condition.hpp"
#include "task.hpp"
#include <queue>

using std::queue;

using ElemType = Task *;

class TaskQueue
{
public:
    TaskQueue(size_t capacity);//队列容量

    bool empty()const;
    bool full()const;
    void push(const ElemType &elem);
    ElemType pop();
    void weakup();

private:
    size_t _capacity;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};

#endif
