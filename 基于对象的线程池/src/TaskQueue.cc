#include "TaskQueue.hpp"

TaskQueue::TaskQueue(size_t capacity)
: _capacity(capacity)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{}

bool TaskQueue::empty() const{
	return _que.size() == 0;
}

bool TaskQueue::full() const{
	return _que.size() == _capacity;
}
//运行在生产者线程
void TaskQueue::push(const ElemType & elem){
	//autolock是一个局部对象
	{ //加上语句块的方式，可以精准控制加锁范围
	MutexLockGuard autolock(_mutex);
	/* _mutex.lock(); */
	while(full()) { //使用while是为了防止出现异常(虚假)唤醒的问题
		_notFull.wait();
	}
	//....exit

	_que.push(elem);
	}
	//通知消费者取数据
	_notEmpty.notify();

	/* _mutex.unlock(); */
}

//运行在消费者线程
ElemType TaskQueue::pop(){
	MutexLockGuard autolock(_mutex);
	while(_flag && empty()) {
		_notEmpty.wait();
	}

	if(_flag) {
		ElemType tmp = _que.front();
		_que.pop();
		//通知生产者线程放入数据
		_notFull.notify();

		return tmp;
	} else {
		return nullptr;
	}
}

void TaskQueue::wakeup(){
	_flag = false;
	_notEmpty.notifyAll();
}
