#include <iostream>

using std::cout;
using std::endl;

class base{
public:
    virtual void print(){
        cout << "base print" << endl;
    }
    virtual void hello(){
        cout << "base hello" << endl;
    }
private:
    int num;
};

class base1 : public base{
public:
    void print(){
        cout << "base1 print" << endl;
    }
    virtual void test(){
        cout << "base1 test" << endl;
    }
private:
    int num;
};


int main()
{
    base1 b1;

    base* base = &b1;
    base->print(); // base1 print
    base->hello(); // base hello
    //base->test(); // OOPS! 子类并不是实现了父类虚函数 在子类虚表中并不能找到这个函数

// 子类虚表
// base::hello()
// base1::print()
// base1::test()

    return 0;
}
