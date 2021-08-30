~~~c++
#include <iostream>

using std::cout;
using std::endl;

void print(const char* s){
    while(*s){
        if(*s == '%' && *(++s) != '%'){
            throw std::runtime_error("invalid format string: missing arguments");
        }
        cout << *s++;
    }
}

template<class T, class... Args>
void print(const char* s, T value, Args... args){
    while(*s){
        if(*s == '%' && *(++s) != '%'){
            cout << value;
            print(++s, args...);
            return;//这个函数执行完了
        }
        cout << *s++;//保证输出中间的空格啥的分隔符
    } 
    throw std::logic_error("extra arguments provided printf");
}

int main()
{
    int *pi = new int;
    print("%d %s %p %f \n", 15, "This is Ace", pi, 3.14159);
    //print("%d %s %p %f %d\n", 15, "This is Ace", pi, 3.14159); //错误情况下
    return 0;
}

~~~

