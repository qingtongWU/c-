# c++05new&输入流&内存对齐

## new和delete表达式

~~~c++
//内置类型没有构造和析构的说法
1.new表达式的工作步骤：
	1). 调用名为operator new的标准库函数，分配足够大的原始的未类型化的内存(静态的)，以保存指定类型的一个对象；
	2). 运行该类型的一个构造函数初始化对象；
	3). 返回指向新分配并构造的构造函数对象的指针。
    
2. delete表达式的工作步骤
	2.)调用析构函数，回收对象中数据成员所申请的资源,释放空间；//对象中的数据成员
	3.)调用名为operator delete的标准库函数释放该对象所用的内存. //对象
注意：静态成员函数，写在类内，只针对类内的new和delete表达式， 
	 若没有静态数据成员，也可写在类外，会针对全局的new和delete表达式。

3.对象的销毁与析构函数的调用是不是等价的？
    栈对象是等价的，因为栈上没有new和delete;
	堆对象，析构函数的调用只是对象销毁的一个步骤。
~~~

### operator new & operator delete函数的重载版本

~~~c++
//operator new库函数
void *operator new(size_t);
void *operator new[](size_t);
//operator delete库函数
void operator delete(void *);
void operator delete[](void *);
~~~

~~~c++
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class Student
{
public:
    Student(int id, const char *name)
    : _id(id)
    , _name(new char[strlen(name) + 1]())
    {
        cout << "Student(int, const char *)" << endl;
        strcpy(_name, name);
    }

    static void *operator new(size_t sz) //new
    {      
        cout << "void *operator new(size_t)" << endl;
        void *pret = malloc(sz);
        return pret;
    }

    static void operator delete(void  *pret) //delete
    {      
        cout << "void operator delete(void *)" << endl;
        free(pret);
    }


    void print() const
    {
        cout << "id :" << _id << endl
             << "name : " << _name << endl;
    }

    ~Student()
    {
        cout << "~Student()" << endl;
        if(_name)
        {
            delete [] _name;
            _name = nullptr;
        }
    }
private:
    int _id;
    char *_name;
};
int main(int argc, char **argv)
{
    Student *pstu = new Student(123, "Lucy");
    pstu->print();

    delete pstu;
    pstu = nullptr;
    return 0;
}


~~~

## 要求一个类只能创建栈对象

~~~c++
创建栈对象的条件：
	1.构造函数是public的(要保证创建)；
	2.析构函数是public的(也要保证销毁)。
如何只创建栈对象呢？
	将operator new / delete设为私有，其实只要将new设为私有就行，delete也设为私有为了对应
	
~~~

~~~c++
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//要求：只能创建栈对象
//解决方案：将operator new/delete设计为私有的(private)
class Student
{
public:
    Student(int id, const char *name)
    : _id(id)
    , _name(new char[strlen(name) + 1]())
    {
        cout << "Student(int, const char *)" << endl;
        strcpy(_name, name);
    }

private:
    static void *operator new(size_t sz)
    {
        
        cout << "void *operator new(size_t)" << endl;
        void *pret = malloc(sz);

        return pret;
    }

    static void operator delete(void  *pret)
    {
       
        cout << "void operator delete(void *)" << endl;
        free(pret);
    }

public:
    void print() const
    {
        cout << "id :" << _id << endl
             << "name : " << _name << endl;
    }

    ~Student()
    {
        cout << "~Student()" << endl;
        if(_name)
        {
            delete [] _name;
            _name = nullptr;
        }
    }
private:
    int _id;
    char *_name;
};
int main(int argc, char **argv)
{

    Student stu(222, "xiaoming");//栈对象
    return 0;
}


~~~

## 要求一个类只能创建堆对象

~~~c++
析构函数设为private即可(栈对象创建不了)
在类内delete,只调用析构函数，不能保证对象被干掉，要delete this，才能销毁对象本身，对象销毁的时候，会自动调用析构函数的。
~~~

~~~c++
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//要求：只能创建堆对象
//解决方案：析构函数设置为私有的
class Student
{
public:
    Student(int id, const char *name)
    : _id(id)
    , _name(new char[strlen(name) + 1]())
    {
        cout << "Student(int, const char *)" << endl;
        strcpy(_name, name);
    }

    static void *operator new(size_t sz)
    {
        cout << "void *operator new(size_t)" << endl;
        void *pret = malloc(sz);

        return pret;
    }

    static void operator delete(void  *pret)
    {
        cout << "void operator delete(void *)" << endl;
        free(pret);
    }

    void print() const
    {
        cout << "id :" << _id << endl
             << "name : " << _name << endl;
    }

    void destroy(/* Student * const this */)
    {
        delete this;//销毁对象本身
    }
private:
    ~Student()
    {
        cout << "~Student()" << endl;
        if(_name)
        {
            delete [] _name;
            _name = nullptr;
        }
    }
private:
    int _id;
    char *_name;
};
int main(int argc, char **argv)
{
    Student *pstu = new Student(123, "Lucy");//堆对象,ok
    pstu->print();
    pstu->destroy();

    return 0;
}


~~~

## c++输入输出流

### 输入输出的含义

~~~c++
1.以终端为对象作为输入输出，也可以以磁盘作为输入输出的对象，磁盘既可以作为输入文件，也可以作为输出文件；
2.程序的输入指的是从输入文件将数据传送给程序(内存)，
  程序的输出指的是从程序(内存)将数据传送给输出文件。
~~~

### 流的概念

~~~c++
C++的 I/O发生在流中，流是字节序列。
如果字节流是从设备（如键盘、磁盘驱动器、网络连接等）流向内存，这叫做输入操作。
如果字节流是从内存流向设备（如显示屏、打印机、磁盘驱动器、网络连接等）这叫做输出操作。
     
就C++程序而言， I/O操作可以简单地看作是从程序移进或移出字节，程序只需要关心是否正确地输出了
字节数据，以及是否正确地输入了要读取字节数据，特定I/O设备的细节对程序员是隐藏的。
~~~

### c++常用流类型

~~~c++
C++的输入与输出包括以下3方面的内容:
1. 标准I/O:对系统指定的标准设备的输入和输出。即从键盘输入数据，输出到显示器屏幕。这种输入输出称为标准的输入输出.
2. 文件I/O:以外存磁盘文件为对象进行输入和输出，即从磁盘文件输入数据，数据输出到磁盘文件。以外存文件为对象的输入输出称为文件的输入输出.
3.串I/O: 对内存中指定的空间进行输入和输出。通常指定一个字符数组作为存储空间（实际上可以利用该空间存储任何信息）。这种输入和输出称为字符串输入输出.
~~~

| 类名                                             | 作用                                               | 头文件   |
| ------------------------------------------------ | -------------------------------------------------- | -------- |
| ios_base                                         | 抽象基类，管理格式化标志和输入/输出异              | iostream |
| ios                                              | 抽象基类，管理任意流缓冲                           | iostream |
| istream<br/>ostream<br/>iostream                 | 通用输入流<br/>通用输出流<br/>通用输入输出流       | iostream |
| ifstream<br/>ofstream<br/>fstream                | 文件输入流<br/>文件输出流<br/>文件输入输出流       | fstream  |
| istringstream<br/>ostringstream<br/>stringstream | 字符串输入流<br/>字符串输出流<br/>字符串输入输出流 | sstream  |

### 流类型之间的关系

~~~c++
ios是抽象基类，由它派生出istream类和ostream类，iostream类支持输入输出操作，
iostream类是从istream类和ostream类通过多重继承而派生的类。
类ifstream继承了类istream，类ofstream继承了类ostream，类fstream继承了类iostream.
~~~

### 流的状态

~~~c++
1. badbit：
	表示发生系统级的错误，如不可恢复的读写错误。通常情况下一旦badbit被置位，流就无法再使用了。
2. failbit：
	表示发生可恢复的错误，如期望读取一个数值，却读出一个字符等错误。这种问题通常是可以修改的，流还可以继续使用。
3. 当到达文件的结束位置时，eofbit和 failbit都会被置位。
4. goodbit:
	被置位表示流未发生错误。如果badbit、 failbit和eofbit任何一个被置位，则检查流状态的条件会失。
~~~

### 管理流的状态

~~~c++
bool bad() const; //若流的badbit置位，则返回true;否则返回false
bool fail() const; //若流的failbit或badbit置位，则返回true;
bool eof() const; //若流的eofbit置位，则返回true;
bool good() const; //若流处于有效状态，则返回true;
iostate rdstate() const; //获取流的状态
void setstate(iostate state); //设置流的状态
//clear的无参版本会复位所有错误标志位*(重置流的状态)
void clear(std::ios_base::iostate state = std::ios_base::goodbit);
~~~

### 流的通用操作

~~~c++
//----以下输入流操作----
int_type get();//读取一个字符
istream & get(char_type & ch);
//读取一行数据
istream & getline(char_type * s, std::streamsize count, char_type delim
='\n');
//读取count个字节的数据
istream & read(char_type * s, std::streamsize count);
//最多获取count个字节，返回值为实际获取的字节数
std::streamsize readsome(char_type * s, std::streamsize count);
//读取到前count个字符或在读这count个字符进程中遇到delim字符就停止，并把读取的这些东西丢
掉
istream & ignore(std::streamsize count = 1, int_type delim = Traits::eof());
//查看输入流中的下一个字符, 但是并不将该字符从输入流中取走
//不会跳过输入流中的空格、回车符; 在输入流已经结束的情况下，返回 EOF。
int_type peek();
//获取当前流中游标所在的位置
pos_type tellg();
//偏移游标的位置
basic_istream & seekg(pos_type pos);
basic_istream & seekg(off_type off, std::ios::seekdir dir);
//----以下为输出流操作----
//往输出流中写入一个字符
ostream & put(char_type ch);
//往输出流中写入count个字符
ostream & write(const char_type * s, std::streamsize count);
//获取当前流中游标所在的位置
pos_type tellp();
//刷新缓冲区
ostream & flush();
//偏移游标的位置
ostream & seekp(pos_type pos);
ostream & seekp(off_type off, std::ios_base::seekdir dir);
~~~

### 示例代码

~~~c++
#include <iostream>
#include <string>
#include <limits>

using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::string;

void printStreamStatus()
{
    cout << "cin.badbit = " << cin.bad() << endl
         << "cin.failbit = " << cin.fail() << endl
         << "cin.eofbit = " << cin.eof() << endl
         << "cin.goodbit = " << cin.good() << endl;
}
void test()
{
    int number = 10;
    printStreamStatus();
    cin >> number;
    printStreamStatus();
    cin.clear();//重置流的状态
    /* cin.ignore(1024, '\n');//清空缓冲区 */
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清空缓冲区
    printStreamStatus();
    cout << "number = " << number << endl;

    string line;
    cin >> line;
    cout << "line = " << line << endl;
}

void test2()
{
    int number = 0;
    //逗号表达式
    //ctrl + d,可以退出来
    while(cin >> number, !cin.eof())
    {
        if(cin.bad())
        {
            cerr << "The stream is bad" << endl;
            return;
        }
        else if(cin.fail())
        {
            cin.clear();//重置流的状态
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清空缓冲区
            cout << "pls input valid int data" << endl;
        }
        else
        {
            cout << "number = " << number << endl;
        }
    }
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}


~~~



## 内存对齐

### 对齐原因

~~~c++
1. 内存对齐的解释
   对齐规则是按照成员的声明顺序，依次安排内存，其偏移量为成员大小的整数倍，0看做任何成员的整数倍，最后结构体的大小为最大成员的整数倍。
    
2.为啥要内存对齐
    1).平台原因(移植原因)：不是所有的硬件平台都能访问任意地址上的任意数据的；某些硬件平台只能在某些地址处取某些特定类型的数据，否则抛出硬件异常。
	2).性能原因：数据结构(尤其是栈)应该尽可能地在自然边界上对齐。原因在于，为了访问未对齐的内存，处理器需要作两次内存访问；而对齐的内存访问仅需要一次访问。
解释二
原因有这么几点：
	1)、CPU 对内存的读取不是连续的，而是分成块读取的，块的大小只能是1、2、4、8、16 ... 字节；
	2)、当读取操作的数据未对齐，则需要两次总线周期来访问内存，因此性能会大打折扣；
	3)、某些硬件平台只能从规定的相对地址处读取特定类型的数据，否则会产生异常。
       
~~~

### 对齐的三大规则

~~~c++
//#pragma pack(n) 对齐系数
1.数据成员
结构(struct)(或联合(union))的数据成员，第一个数据成员放在offset(偏移)为0的地方，以后每个数据成员的对齐按照 #pragma pack 指定的数值和这个数据成员自身长度中，比较小的那个进行
声明的顺序不一样，内存中占的大小不一样。
    
2.结构(或联合)的整体对齐 -- 最大数据成员的整数倍
在数据成员完成各自对齐之后，结构(或联合)本身也要进行对齐，对齐将按照 #pragma pack 指定的数值和结构(或联合)最大数据成员长度中，比较小的那个进行。
    
3.结构体作为成员
如果一个结构里有某些结构体成员，则内部结构体成员要从成员最大元素大小的整数倍和 #pragma
pack指定的数值中最小的一个的整数倍的地址开始存储。
~~~

### 示例代码

~~~c++
#include <iostream>
/* #pragma pack(4) */

using std::cout;
using std::endl;

struct x
{
    char a;
    int b;
    short c;
    char d;
}MyStructX;//12

struct y
{
    int b;
    char a;
    char d;
    short c;
}MyStructY;//8

struct SS
{
    int a;
    char b;
    short c;
    int d;
    struct FF
    {
        int a1;
        char b1;
        short c1;
        char d1;
    }MyStructFF;

#if 1
    /* char e;//28 */
    int e;
    double ww;//40
#endif
}MyStructSS;


struct DD
{
    int a;
    char b;
    short c;
    int d;

    struct FF
    {
        double a1;
        char b1;
        short c1;
        char d1;
    }MyStructFF;

    char e;//40
    
}MyStructDD;

struct GG
{
    char e[2];
    short h;

    struct A
    {
        int a;
        double b;
        float c;
    }MyStructA;
}MyStructGG;//32



int main(int argc, char **argv)
{
    cout <<"sizeof(MyStructX) = " << sizeof(MyStructX) << endl;
    cout <<"sizeof(MyStructY) = " << sizeof(MyStructY) << endl;
    cout <<"sizeof(MyStructSS) = " << sizeof(MyStructSS) << endl;
    cout <<"sizeof(MyStructDD) = " << sizeof(MyStructDD) << endl;
    cout <<"sizeof(MyStructGG) = " << sizeof(MyStructGG) << endl;
    
    return 0;
}


~~~

