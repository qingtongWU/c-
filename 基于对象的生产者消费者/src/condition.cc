#include "condition.hpp"

Condition::Condition(MutexLock &mutex)
    : _mutex(mutex)
{
    pthread_cond_init(&_cond, nullptr);
}

void Condition::wait()
{
    pthread_cond_wait(&_cond, _mutex.getmutexptr());
}

void Condition::notify() //激发一个
{
    pthread_cond_signal(&_cond);
}

void Condition::notifyall() //激发所有
{
    pthread_cond_broadcast(&_cond);
}

Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}
