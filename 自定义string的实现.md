~~~c
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//自定义String类
class String{
public:
    String()
    /* : _pstr(new char[1]())//第一种方式 */
    : _pstr(nullptr)//初始化
    {
        /* _pstr = nullptr;//称为赋值，不是初始化 */
        cout << "String()" << endl;
    }

    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 1]())
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, pstr);
    }

    //拷贝构造函数
    String(const String & rhs)
    : _pstr(new char[strlen(rhs._pstr) + 1]())//深拷贝
    {
        cout << "String(const String &)" << endl;
        strcpy(_pstr, rhs._pstr);
    }

    //赋值运算符函数
    String &operator=(const String & rhs){
        cout << "String &operator=(const String &)" << endl;
        if(this != &rhs){//1、自复制
            delete [] _pstr;//2、释放左操作数
            _pstr = nullptr;

            _pstr = new char[strlen(rhs._pstr) + 1]();//3、深拷贝
            strcpy(_pstr, rhs._pstr);
        }
        return  *this;//4、返回*this
    }

    ~String(){
        cout << "~String()" << endl;
        if(_pstr){
            delete [] _pstr;
            _pstr = nullptr;
        }
    }

    void print() const{
        if(_pstr){
            cout << _pstr << endl;
        }
    }
private:
    char *_pstr;
};

//测试函数
void test(){
	//空的构造函数（无参构造函数）
    String str1;
    str1.print();
    cout << endl << endl;
    //C风格字符串的构造函数
    String str2 = "Hello,world";//String("Hello,world")
    String str3("wangdao");
    str2.print();       
    str3.print();   
    cout << endl << endl;
    //拷贝构造函数
    String str4 = str3;
    str4.print();
    cout << endl << endl;
    //赋值运算符函数
    str4 = str2;
    str4.print();
}
//主函数入口
int main(void){
    test();
    return 0;
}
~~~

