#ifndef __WD_TASKQUEUE_HPP__
#define __WD_TASKQUEUE_HPP__

#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Task.hpp"
#include <queue>

using ElemType = Task;

class TaskQueue{
public:
	TaskQueue(size_t capacity);

	bool empty() const;
	bool full() const;

	void push(const ElemType & elem);
	ElemType pop();

	void wakeup();

private:
	size_t _capacity;
	std::queue<ElemType> _que;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	bool _flag;
};

#endif
