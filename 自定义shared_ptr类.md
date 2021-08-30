~~~c
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;

//SharedPtr类的实现
template<class T>
class SharedPtr{
private:
    size_t *_count;
    T *_ptr;
public:
    //构造函数
    SharedPtr()
    : _ptr(nullptr)
    , _count(new size_t ) { }
    
    SharedPtr(T *ptr)
    : _ptr(ptr)
    , _count(new size_t)
    {
        *_count = 1;
    }

    //析构函数
    ~SharedPtr(){
        --(*_count);
        if( 0 == *_count){
            delete _ptr;
            delete _count;
            _ptr = nullptr;
            _count = nullptr;
        }
    }

    //拷贝构造函数
    SharedPtr(const SharedPtr& ptr){
        _count = ptr._count;
        _ptr = ptr._ptr;
        ++(*_count);
    }

    //拷贝赋值运算符函数
    void operator=(const SharedPtr& ptr){
        SharedPtr(std::move(ptr));
    }

    //移动构造函数
    SharedPtr(SharedPtr&& ptr)
    : _ptr(ptr._ptr)
    , _count(ptr._count)
    {
        ++(*_count);
    }

    //移动赋值运算符函数
    void operator=(SharedPtr&& ptr){
        SharedPtr(std::move(ptr));
    }

    //解引用
    T& operator*(){return *_ptr;}

    //箭头运算符
    T* operator->(){return _ptr;}

    //重载bool操作符
    operator bool(){return _ptr == nullptr;}

    //得到地址
    T* get(){return _ptr;}
    
    //得到引用计数
    size_t use_count(){return *_count;}
    
    bool unique(){return *_count == 1;}
    void swap(SharedPtr& ptr){std::swap(*this, ptr);}
};

//测试函数
void test(){
    SharedPtr<int> sp(new int(10));
    cout << "*sp = " << *sp << endl;
    cout << "sp.get() = " << sp.get() << endl;//得到地址
    cout << "sp.use_count() = " << sp.use_count() << endl;//得到引用计数

    cout << endl;
    SharedPtr<int> sp2(sp);
    cout << "*sp2 = " << *sp2 << endl;
    cout << "*sp = " << *sp << endl; 
    cout << "sp2.get() = " << sp2.get() << endl;
    cout << "sp.get() = " << sp.get() << endl;
    cout << "sp2.use_count() = " << sp2.use_count() << endl;
    cout << "sp.use_count() = " << sp.use_count() << endl;
}

void test1(){
    shared_ptr<int> sp(new int(10));
    cout << "*sp = " << *sp << endl;
    cout << "sp.get() = " << sp.get() << endl;//得到地址
    cout << "sp.use_count() = " << sp.use_count() << endl;//得到引用计数

    cout << endl;
    shared_ptr<int> sp2(sp);
    cout << "*sp2 = " << *sp2 << endl;
    cout << "*sp = " << *sp << endl; 
    cout << "sp2.get() = " << sp2.get() << endl;
    cout << "sp.get() = " << sp.get() << endl;
    cout << "sp2.use_count() = " << sp2.use_count() << endl;
    cout << "sp.use_count() = " << sp.use_count() << endl;
}
int main(){
    cout << "自定义的SharedPtr" << endl;
    test();
    cout << endl;
    cout << "内置的shared_ptr" << endl;
    test1();
    return 0;
}

//结果如下
自定义的SharedPtr
*sp = 10
sp.get() = 0x560c3d746280
sp.use_count() = 1

*sp2 = 10
*sp = 10
sp2.get() = 0x560c3d746280
sp.get() = 0x560c3d746280
sp2.use_count() = 2
sp.use_count() = 2

内置的shared_ptr
*sp = 10
sp.get() = 0x560c3d7462a0
sp.use_count() = 1

*sp2 = 10
*sp = 10
sp2.get() = 0x560c3d7462a0
sp.get() = 0x560c3d7462a0
sp2.use_count() = 2
sp.use_count() = 2

~~~

