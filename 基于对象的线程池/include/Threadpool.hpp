#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "TaskQueue.hpp"
#include "Task.hpp"
#include <vector>
#include <memory>
using std::unique_ptr;
using std::vector;

class Thread;

class Threadpool{
public:
	Threadpool(size_t , size_t);
	~Threadpool();

	void start();
	void stop();

	void addTask(Task && task);

private:
	void workerDoTask();
	Task getTask();

private:
	size_t _threadNum;
	size_t _capacity;
	vector<unique_ptr<Thread>> _threads;
	TaskQueue _taskque;
	bool _isExit;
};

#endif
