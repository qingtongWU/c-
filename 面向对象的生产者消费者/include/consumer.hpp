#ifndef __CONSUMER_HPP__
#define __CONSUMER_HPP__

#include "taskqueue.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

class Consumer
{
public:
    void consume(TaskQueue &taskque)
    {
        int count = 15;

        while (count-- > 0)
        {
            int num = taskque.pop();
            cout << "consumer consume a number " << num << endl;
            sleep(1);
        }
    }
};

#endif
