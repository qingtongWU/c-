#include "threadpool.hpp"

#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

class Test
: public Task
{
public:
	void process()
	{
		srand(time(nullptr));
		int num = rand() % 100;
		cout << "num= " << num << endl;
		sleep(1);//睡一秒
	}
};

void test() 
{

	ThreadPool threadpool(6, 8);
	threadpool.start();

	unique_ptr<Task> task(new Test());
	int count = 10;//添加10个任务
    
	while(count > 0) {
		threadpool.addTask(task.get());
		cout << "count= " << count << endl;
        count --;
	}

	threadpool.stop();

} 
int main(void)
{
	test();
	return 0;
}
