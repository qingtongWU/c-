~~~c
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;
using std::ostream;

class String;
//定义一个类，重载运算符=和<<
//为了解决重载=运算符，s1[0]='H'时，两边都是char的问题
class Diff
{
public:
    Diff(size_t sz, String &str)
        : _sz(sz), _str(str) {} //初始化列表

    Diff &operator=(const char &rh);
    friend ostream &operator<<(ostream &os, const Diff &rs);

private:
    size_t _sz;
    String &_str;
};
class String
{
public:
    //为了这个类中调用，声明友元
    friend ostream &operator<<(ostream &os, const Diff &rs);
    friend class Diff;//友元类

    //调用成员访问运算符的时候访问 Diff类
    Diff operator[](size_t sz)
    {
        return Diff(sz, *this);
    }
    //构造函数
    String()
        : _pstr(new char[5]() + 4)
    {
        *(int *)(_pstr - 4) = 1;
    }
    //有参构造函数
    String(const char *pstr)
        : _pstr(new char[strlen(pstr) + 5]() + 4)
    {
        strcpy(_pstr, pstr);
        *(int *)(_pstr - 4) = 1;
    }
    //拷贝构造函数
    String(const String &rhs)
        : _pstr(rhs._pstr)
    {
        ++*(int *)(_pstr - 4);
    }
    //析构函数
    ~String()
    {
        --*(int *)(_pstr - 4);
        if (0 == *(int *)(_pstr - 4))
        {
            delete[](_pstr - 4);
        }
    }

    String &operator=(const String &rhs)

    {
        if (this != &rhs) //防止自我复制
        {
            --*(int *)(_pstr - 4); //引用计数减1后若为0，则删除
            if (0 == *(int *)(_pstr - 4))
            {
                delete[](_pstr - 4);
            }
            _pstr = rhs._pstr;     //浅拷贝
            ++*(int *)(_pstr - 4); //引用计数加1
        }
        return *this;
    }

    friend ostream &operator<<(ostream &os, const String &rhs);

    //得到字符串首地址
    const char *c_str()
    {
        return _pstr;
    }
    //获取引用计数
    int getRefcount()
    {
        return *(int *)(_pstr - 4);
    }
    //得到字符串长度
    size_t size()
    {
        return strlen(_pstr);
    }

private:
    char *_pstr;
};

//成员访问运算符的时候调用的赋值函数
Diff &Diff::operator=(const char &rh)
{
    if (_sz < _str.size()) //界内
    {
        if (_str.getRefcount() > 1) //是共享的
        {
            char *ptmp = new char[_str.size() + 5]() + 4;
            strcpy(ptmp, _str._pstr);
            --*(int *)(_str._pstr - 4);
            _str._pstr = ptmp;
            *(int *)(_str._pstr - 4) = 1;
        }
        _str._pstr[_sz] = rh;
    }
    return *this;
}
//成员访问运算符的时候调用的输出流函数
ostream &operator<<(ostream &os, const Diff &rs)
{
    os << rs._str._pstr[rs._sz];
    return os;
}

ostream &operator<<(ostream &os, const String &rhs)
{
    if (rhs._pstr)
    {
        os << rhs._pstr;
    }
    return os;
}

//测试用例
void test()
{
    String s1("hello");
    String s2 = s1;
    String s3("world");

    s3 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;

    cout << endl
         << "对s3[0]执行写操作" << endl;
    s3[0] = 'H';
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;

    cout << endl
         << "对s1[0]执行读操作" << endl;
    cout << "s1[0] = " << s1[0] << endl;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;
}
int main()
{
    test();
    return 0;
}

//结果如下
wuqingtong@wuqingtong-virtual-machine:~/c++/day10$ ./a.out
s1 = hello
s2 = hello
s3 = hello
s1.getRefcount() = 3
s2.getRefcount() = 3
s3.getRefcount() = 3

对s3[0]执行写操作
s1 = hello
s2 = hello
s3 = Hello
s1.getRefcount() = 2
s2.getRefcount() = 2
s3.getRefcount() = 1//写操作的时候，引用计数变化了

对s1[0]执行读操作
s1[0] = h
s1 = hello
s2 = hello
s3 = Hello
s1.getRefcount() = 2
s2.getRefcount() = 2
s3.getRefcount() = 1//读操作的时候，引用计数没变哦

~~~

