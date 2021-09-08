#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include <pthread.h>
#include "mutex.hpp"

class Condition
{

public:
    Condition(MutexLock &mutex);
   
    void wait();
    void notify(); //激发一个 
    void notifyall(); //激发所有

    ~Condition();
private:
    pthread_cond_t _cond;
    MutexLock  &_mutex;
};

#endif
