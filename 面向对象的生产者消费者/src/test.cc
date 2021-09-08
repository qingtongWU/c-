#include "Thread.hpp"
#include "producter.hpp"
#include "consumer.hpp"
#include "taskqueue.hpp"

#include <memory>
#include <iostream>

using std::cout;
using std::endl;
using std::unique_ptr;

void test()
{
    TaskQueue taskque(10);

    Producter per;
    Consumer cer;

    unique_ptr<Thread> producer(new Thread(    
        std::bind(&Producter::produce,per,std::ref(taskque))));

    unique_ptr<Thread> consumer(new Thread(    
        std::bind(&Consumer::consume,cer,std::ref(taskque))));

    producer->start();
    consumer->start();

    producer->join();
    consumer->join();   
}

int main(void)
{
    test();
    return 0;
}
