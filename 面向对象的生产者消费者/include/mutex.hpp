#ifndef __mutex_hpp__
#define __mutex_hpp__

#include <pthread.h>

class MutexLock
{
public:   
    MutexLock();
    
    void lock();    
    void unlock();

    pthread_mutex_t * getmutexptr();
    
    ~MutexLock();   
private:
    pthread_mutex_t _mutex;
};

//用对象的生命周期管理资源
class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock & mutex)
	: _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard() {	_mutex.unlock();	}

private:
	MutexLock & _mutex;

};

#endif