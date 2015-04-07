#include <iostream>

using std::cout;
using std::endl;

class base{
private:
    int num;
};

class base1 : public base{
private:
    int num;
};


int main()
{
    base b;
    base1 b1;

    cout << sizeof(b) << endl;  // 4
    cout << sizeof(b1) << endl; // 8

    return 0;
}