# c++11继承，set & map

## 继承

### 基本概念

~~~c++
通过继承，我们可以用原有类型来定义一个新类型，定义的新类型既包含了原有类型的成员，也
能自己添加新的成员，而不用将原有类的内容重新书写一遍.

基类(父类)
派生类(子类)
    
class 派生类
: public/protected/private 基类
{
};

派生类的生成过程：
1. 吸收基类的成员
2. 改造基类的成员
3. 添加自己新的成员
~~~

### 继承的局限

~~~c++
不论何种继承方式，下面这些基类的特征是不能从基类继承下来的：
1. 构造函数
2. 析构函数
3. 用户重载的operator new/delete运算符
4. 用户重载的operator=运算符
5. 友元关系
~~~

### 派生方式对基类成员的访问权限

| 继承方式 | 基类成员访问权限 | 在派生类中访问权限 | 派生类对象访问 |
| -------- | ---------------- | ------------------ | -------------- |
|          | public           | public             | 可直接访问     |
| 公有继承 | protected        | protected          | 不可直接访问   |
|          | private          | 不可直接访问       | 不可直接访问   |
|          | public           | protected          |                |
| 保护继承 | protected        | protected          | 不可直接访问   |
|          | private          | 不可直接访问       |                |
|          | public           | private            |                |
| 私有继承 | protected        | private            | 不可直接访问   |
|          | private          | 不可直接访问       |                |

~~~c++
通过继承，除了基类私有成员以外的其它所有数据成员和成员函数，派生类中可以直接访问。
private成员是私有成员，只能被本类的成员函数所访问，派生类和类外都不能访问。
public成员是公有成员，在本类、派生类和外部都可访问。
protected成员是保护成员，只能在本类和派生类中访问，是一种区分血缘关系内外有别的成员。

总结：派生类的访问权限规则如下：
1．不管以什么继承方式，派生类内部都不能访问基类的私有成员。
2．不管以什么继承方式，派生类内部除了基类的私有成员不可以访问外，其他的都可以访问。
3．不管以什么继承方式，派生类对象除了公有继承基类中的公有成员可以访问外，其他的一律不能访问
~~~

### 派生类对象的构造

~~~c++
//构造由内而外
创建派生类对象时，系统首先通过派生类的构造函数来调用基类的构造函数，完成基类成员的初始化，而后对派生类中新增的成员进行初始化。
    基类中的无参构造函数中的数据成员，是一些随机的值，为了方便看可以写成0.
    
1.如果派生类有显式定义构造函数，而基类没有，则创建派生类的对象时，先调用基类缺省的无参构造函数，(初始化继承基类的数据成员)，再自动调用派生类相应的构造函数。

    
2.如果派生类没有显式定义构造函数而基类有显示定义构造函数，则基类必须拥有默认(无参)构造函数。
    
3.如果派生类有构造函数，基类有默认构造函数，则创建派生类的对象时，基类的默认构造函数会自
动调用，如果你想调用基类的有参构造函数，必须要在派生类构造函数的初始化列表中显示调用基
类的有参构造函数。
    
4.如果派生类和基类都有构造函数，但基类没有默认的无参构造函数，即基类的构造函数均带有参数,
则派生类的每一个构造函数必须在其初始化列表中显示的去调用基类的某个带参的构造函数.如果派生类的初始化列表中没有显示调用则会出错，因为基类中没有默认的构造函数。
    
总结：
  a.在创建派生类对象的时候，会调用派生类相应的构造函数，
//b.为了完成从基类这里吸收过来的数据成员的初始化，所以会调用基类的构造函数完成初始化工作， 
  c.有参的必须显式调用，无参的可以隐式调用，若基类中没有声明无参的，可能会出错；
  d.然后在初始化派生类自己的数据成员进而执行派生类自己的构造函数的函数体。
    
调用顺序：
    1. 完成对象所占整块内存的开辟，由系统在调用构造函数时自动完成。
	2. 调用基类的构造函数完成基类成员的初始化。
	3. 若派生类中含对象成员、const成员或引用成员，则必须在初始化表中完成其初始化。
	4. 派生类构造函数体执行
~~~

### 派生类对象的销毁

~~~c++
//析构由外而内
析构函数的调用顺序：
1. 先调用派生类的析构函数
2. 再调用派生类中成员对象的析构函数
3. 最后调用普通基类的析构函数
~~~

### 多基继承

~~~c++
1.派生类多继承基类的时候，在每个类前加继承方式，否则默认为私有；
class C
: public A
,public B,public D
{}；

2. 构造函数的调用顺序与派生类构造函数的初始化表达式中的顺序没有关系，只和在定义派生类的时候，基类被继承的顺序有关。
    
3.基类的构造函数显式的写出来。
    

~~~

### 二义性问题

~~~c++
1.成员名访问二义性
    只需要在调用时，指明要调用的是某个基类的成员函数即可，即 类名+::

2.菱形继承的二义性问题
  B,C继承A,D继承B和C
class A
{
public:
	void setNumber(long number)
	{
		_number = number;
	}
private:
	long _number;
};

class B
: virtual public A //采用虚拟继承
{
};
class C
: virtual public A //采用虚拟继承
{
};

class D //使得D中继承的_number 只有一份
: public B
, public C
{
};
//多的8个字节是指针，存储对象前的虚基指针，D中有两个指针，B中有一个，C中一个。
~~~

### 基类和派生类之间的相互转换

~~~c++
派生类比基类要大
1. 类型适应
    指两种类型之间的关系，说A类适应B类是指A类的对象能直接用于B类对象所能应用的场合，派
 
2.派生类适应于基类
  	派生类的对象适应于基类对象，派生类对象的指针和引用也适应于基类对象的指针和引用。
    a. 可以把派生类的对象赋值给基类的对象
 	b. 可以把基类的引用绑定到派生类的对象(引用的底层实现就是指针)
	c. 可以声明基类的指针指向派生类的对象 (向上转型)
	d.  a,b,c反过来不行哦
    
原因：
    a.为啥派生类对象可以向基类对象转化，却不能反过来？
   	派生类继承了基类的数据成员，也有派生类自己的数据成员，所以派生类中的基类的数据成员可以对基类中的数据成员进行初始化哦；若基类转化为派生类，则派生类自己的数据成员得不到初始化。
    b.为啥基类指针可以指向派生类对象？
    基类指针可以操纵8个字节，操纵派生类中继承的基类数据成员；而派生类指针有16个字节，基类对象只有8个字节，派生类指针还可以操纵不属于基类对象的8个字节，属于内存的操作不合法问题。
    
也就是说如果函数的形参是基类对象或者基类对象的引用或者基类对象的指针类型，在进行函数调用
时，相应的实参可以是派生类对象。   

~~~

### 向下转型

~~~c++
	Base base3(111);
    Derived derived3(222, 333);
    Derived *pderived3 = static_cast<Derived *>(&base3);//向下转型(不安全的)

    Base *pbase3 = &derived3;
    Derived *pderived4 = static_cast<Derived *>(pbase3);//向下转型（安全的）
~~~



## 关联容器 set

~~~c++
//c语言一次返回两个以上参数的时候 
1. 特点：有序(默认升序)，去重(key唯一),
   底层用的红黑树，为了保护红黑树的结构稳定，不允许修改，所以set不支持下标访问。
2.初始化方式：
a.
set<int> number = {1,3,5,7,9,3,5,7,2};
for(auto &elem : number)
{
    cout<<elem<<" ";
}
cout<<endl;

b.
int arr[10] = {1,3,5,7,2,4,2};
set<int> number(arr,arr+10); //这个区间是左闭右开的哦
set<int>:: iterator it;//迭代器
for(it=number.begin();it!=number.end();++it)
{
    cout<<*it<<" ";
}
cout<<endl;

3.查找 count(),find()
size_t cnt1 = number.count(8); //不存在为0
size_t cnt2 = number.count(3); //存在则统计有几个

auto it2 = number.find(3);//找到则返回该元素所在位置的迭代器，找不到返回空
set<int>:: iterator it2 = number.find(8);
if(it2 == number.end()) //为空
{
    cout<<"该元素不存在set中"<<endl;
}
else
{
    cout<<"该元素存在set中"<<*it2<<endl;
}
 
4.插入 -- insert()
std::pair
Defined in header<utility>
template<
    class T1,
	class T2
>struct pair;

std::pair<iterator,bool> insert(const value_type& value);

pair<set<int>::iterator,bool> ret = number.insert(6);
if(ret.second)
{
    cout<<"该元素插入成功"<<*ret.first<<endl;
}
else
{
    cout<<"该元素插入失败"<<endl;
}

void test()
{
    pair<int,string> number{1,"hello"};
    cout<<number.first<<" "<<number.second<<endl;
}
~~~

## 关联容器 map

~~~c++
特点： key唯一，不可重复，但value可重复，默认时按key升序排列，底层也用的红黑树。
下标访问的时候：有则输出，没有则为空，并插入map中；有读和写的功能。
1.初始化
map<string,string> number = {
    {"021","上海"},
    {"207","武汉"},
    {"0755","深圳"},
    pair<string,string>("0653","南京")
}；
2. 遍历
a.
for(auto &elem : number)
{
    cout<<elem.first<<" "<<elem.second<<endl;
}

b.
map<string,string>::itreator it;
for(it = number.begin();int!=number.end();++it)
{
    cout<<it->first<<" "<<it->second<<endl;
}

3.插入 -- 注意参数和返回值
 /* pair<map<string, string>::iterator, bool>  ret */ 
    /*  = number.insert(pair<string, string>("0999", "新疆")); */
    /* pair<map<string, string>::iterator, bool>  ret */ 
    /*     = number.insert({"0699", "新疆"}); */

    //注意 auto的妙用哦
    auto ret = number.insert(pair<string, string>("0999", "新疆"));
    if(ret.second)
    {
        cout << "该元素不在map中，插入成功 " 
             << ret.first->first << "  "
             << ret.first->second << endl;
    }
    else
    {
        cout << "该元素存在，map中， 插入失败" << endl;
    }
    for(auto &elem : number)
    {
        cout << elem.first << "  " << elem.second << endl;
    }
    cout << endl << endl;

    cout << endl << "map的下标访问" << endl;
    cout << "number[\"021\"] = " << number["021"] << endl;
    cout << "number[\"029\"] = " << number["029"] << endl;
    for(auto &elem : number)
    {
        cout << elem.first << "  " << elem.second << endl;
    }  
    cout << endl << endl;

    number["022"] = "河南";//插入操作,调用了下标访问运算符和赋值运算符
    //number.operator[]("022").operator=("河南");  
~~~

## 派生类对象间的复制控制

~~~c++
1. 如果用户定义了基类的拷贝构造函数，而没有定义派生类的，那么在用一个派生类对象初始化新的派生类对象时，两对象间的派生类部分执行缺省的行为，而两对象间的基类部分执行用户定义的基类拷贝构造函数。
    
2. 如果用户重载了基类的赋值运算符函数，而没有重载派生类的，那么在用一个派生类对象给另一个已经存在的派生类对象赋值时，两对象间的派生类部分执行缺省的赋值行为，而两对象间的基类部分执行用户定义的重载赋值函数。
    
3. 如果用户定义了派生类的拷贝构造函数或者重载了派生类的对象赋值运算符=，则在用已有派生类
对象初始化新的派生类对象时，或者在派生类对象间赋值时，将会执行用户定义的派生类的拷贝构
造函数或者重载赋值函数，而不会再自动调用基类的拷贝构造函数和基类的重载对象赋值运算符，
这时，通常需要用户在派生类的拷贝构造函数或者派生类的赋值函数中显式调用基类的拷贝构造或
赋值运算符函数
    
总结：派生类没有定义，基类定义了，要基类，自己缺省；派生类自己定义了，要显示调用基类。
~~~



~~~c++
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base()
    : _pbase(nullptr)
    {
        cout << "Base()" << endl;
    }

    Base(const char *pbase)
    : _pbase(new char[strlen(pbase) + 1]())
    {
        cout << "Base(const char *)" << endl;
        strcpy(_pbase, pbase);
    }

    Base(const Base &rhs)
    : _pbase(new char[strlen(rhs._pbase) + 1]())
    {
        cout << "Base(const Base &)" << endl;
        strcpy(_pbase, rhs._pbase);
    }

    Base &operator=(const Base &rhs)
    {
        cout << "Base &operator=(const Base &)" << endl;
        if(this != &rhs)
        {
            delete [] _pbase;
            _pbase = nullptr;

            _pbase = new char[strlen(rhs._pbase) + 1]();
            strcpy(_pbase, rhs._pbase);
        }

        return *this;
    }

    ~Base()
    {
        cout << "~Base()" << endl;
        if(_pbase)
        {
            delete [] _pbase;
            _pbase = nullptr;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Base &rhs);
private:
    char *_pbase;
};

std::ostream &operator<<(std::ostream &os, const Base &rhs)
{
    if(rhs._pbase)
    {
        os << rhs._pbase;
    }

    return os;
}

class Derived
: public Base
{
public:
    Derived(const char *pbase, const char *pderived)
    : Base(pbase)
    , _pderived(new char[strlen(pderived) + 1]())
    {
        cout << "Derived(const char *, const char *)" << endl;
        strcpy(_pderived, pderived);
    }

    Derived(const Derived &rhs)
    : Base(rhs)//显示调用拷贝构造函数
    , _pderived(new char[strlen(rhs._pderived) + 1]())
    {
        cout << "Derived(const Derived &)" << endl;
        strcpy(_pderived, rhs._pderived);
    }

    Derived &operator=(const Derived &rhs)
    {
        cout << "Derived &operator=(const Derived &)" << endl;
        if(this != &rhs)
        {
            Base::operator=(rhs);//显示调用基类的赋值运算符函数

            delete [] _pderived;
            _pderived = nullptr;

            _pderived = new char[strlen(rhs._pderived) + 1]();
            strcpy(_pderived, rhs._pderived);
        }

        return *this;

    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
        if(_pderived)
        {
            delete [] _pderived;
            _pderived = nullptr;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Derived &rhs);
private:
    char *_pderived;
};

std::ostream &operator<<(std::ostream &os, const Derived &rhs)
{
    const Base &base = rhs;//基类的引用绑定到派生类对象
    os << base << ", " << rhs._pderived;
    return os;
}

int main(int argc, char **argv)
{
    Derived d1("hello", "world");
    cout << "d1 = " << d1 << endl;

    cout << endl;
    Derived d2(d1);
    cout << "d1 = " << d1 << endl;
    cout << "d2 = " << d2 << endl;

    cout << endl;
    Derived d3("hubei", "wuhan");
    cout << "d3 = " << d3 << endl;
    d3 = d1;
    cout << "d1 = " << d1 << endl;
    cout << "d3 = " << d3 << endl;

    return 0;
}

~~~

