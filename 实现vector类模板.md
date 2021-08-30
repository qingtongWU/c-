~~~c
Vector模型
______________________________
|_|_|_|_|_|____________________|
↑          ↑                    ↑
_start   _finish            _end_of_storage
~~~

~~~c
#include <iostream>
#include <memory>
#include <string>

using std::allocator;
using std::cout;
using std::endl;
using std::string;

//自定义vector类模板
template <typename T>
class Vector{
public:
    Vector();
    ~Vector();

    void push_back(const T &);
    void pop_back();
    int size();
    int capacity();
    
    /* 不用重载下标访问运算符的时候，这样写也行
    //typedef T * iterator;//c中写法
    using iterator = T *;//c++11中的写法
    
    T * begin(){
        return _start;
    }
    T * end(){
        return _finish;
    }
    */ 
    T &operator[](size_t idx); //重载下标访问运算符
private:
    void reallocate(); //重新分配内存,动态扩容要用的
private:
    static std::allocator<T> _alloc;
    T *_start;          //指向数组中的第一个元素
    T *_finish;         //指向最后一个实际元素之后的那个元素
    T *_end_of_storage; //指向数组本身之后的位置
};
//静态成员的定义
template <typename T>
allocator<T> Vector<T>::_alloc;

//vector的无参构造函数
template <typename T>
Vector<T>::Vector()
    : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr)
	{ }

//析构函数
template <typename T>
Vector<T>::~Vector(){
    if (_start){
        while (_start != _finish){
            _alloc.destroy(--_finish);
        }
        _alloc.deallocate(_start, capacity()); //回收空间    
    }
     _finish = nullptr;
     _start = nullptr;
     _end_of_storage = nullptr;
}

//在vector的尾部插入元素
template <typename T>
void Vector<T>::push_back(const T &val){
    if (size() == capacity()){
        reallocate(); //重新分配内存
    }
    if(size() < capacity()){
        _alloc.construct(_finish++, val);
    }   
}

//在vector的尾部删除元素
template <typename T>
void Vector<T>::pop_back(){
    if (size() > 0){
        _alloc.destroy(--_finish);
    }
}

//获取vector中的元素数量
template <typename T>
int Vector<T>::size(){
    return _finish - _start;
}

//获取vector的容量
template <typename T>
int Vector<T>::capacity(){
    return _end_of_storage - _start;
}

//动态分配内存
template <typename T>
void Vector<T>::reallocate(){
    size_t oldCapacity = size(); //当size() = capacity()的时候才分配内存
    size_t newCapacity = (size() == 0) ? 1 : size() * 2;
    T *tmp = _alloc.allocate(newCapacity); //分配新的空间

    //回收旧空间
    if (_start){
        std::uninitialized_copy(_start, _finish, tmp); //旧空间数据拷贝到新空间
        while (_start != _finish){
            _alloc.destroy(--_finish);//一次只能销毁一个元素
        }
        _alloc.deallocate(_start, oldCapacity); //回收旧空间
    }
    //新空间指针的初始化
    _start = tmp;
    _finish = _start + oldCapacity;
    _end_of_storage = _start + newCapacity;
}

//重载的下标访问运算符
template <typename T>
T &Vector<T>::operator[](size_t idx){
    return _start[idx];
}

//测试函数
void test(){
    Vector<int> vec;
    cout << "size()=" << vec.size() << endl;
    cout << "capacity()=" << vec.capacity() << endl;
    for (int i = 0; i < 10; ++i){
        vec.push_back(i);
    }
    cout << endl<<"size()=" << vec.size() << endl;
    cout << "capacity()=" << vec.capacity() << endl;

    vec.pop_back();
    vec.pop_back();
    vec.pop_back();

    cout << endl<<"size()=" << vec.size() << endl;
    cout << "capacity()=" << vec.capacity() << endl;
    cout << "vctor中的元素：" << endl;
  
    /*for(auto &elem : vec)
    {
        cout << elem << " ";
    }*/
    
    for (size_t idx = 0; idx != vec.size(); ++idx){
        cout << vec[idx] << " ";
    }
    cout << endl;
}

void test2(){
    Vector<string> vec;
    cout <<"size()=" << vec.size() << endl;
    cout << "capacity()=" << vec.capacity() << endl;

    vec.push_back("world");
    vec.push_back("miss");
    vec.push_back("you");
    vec.push_back("hello");

    cout << endl<<"size()=" << vec.size() << endl;
    cout << "capacity()=" << vec.capacity() << endl;

    vec.pop_back();

    cout << endl<<"size()=" << vec.size() << endl;
    cout << "capacity()=" << vec.capacity() << endl;

    cout << "vctor中的元素：" << endl;
    for (size_t idx = 0; idx != vec.size(); ++idx){
        cout << vec[idx] << " ";
    }
    cout << endl;
}

int main(){
    cout << "int类型" << endl;
    test();
    cout<<endl<<endl;
    cout <<"string类型" << endl;
    test2();
    return 0;
}
~~~

