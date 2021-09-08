#ifndef __WD_CONDITION_HPP__
#define __WD_CONDITION_HPP__
#include "Noncopyable.hpp"
#include <pthread.h>

class MutexLock;
class Condition 
: Noncopyable {
public:
	Condition(MutexLock &);
	~Condition();

	void wait();
	void notify();
	void notifyAll();

private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
}; 
#endif
