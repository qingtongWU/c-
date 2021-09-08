#ifndef _PRODUCTER_HPP__
#define _PRODUCTER_HPP__

#include "taskqueue.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

class Producter
{
public:
    void produce(TaskQueue &taskque)
    {
        int count = 15;
        srand(time(nullptr));
        while (count > 0)
        {
            int num = rand() % 100;
            cout << "producter producting a number " << num <<endl;
            taskque.push(num);
            count--;
            sleep(1);
        }
    }
};

#endif
