#if 0 // C Code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// C语言中一个结构体的变量本质上是对应了一块内存
struct student{
    int    id;
    double score;
};

void set_student_id(struct student* stu, int student_id){
    assert(stu);
    stu->id = student_id;
}

void set_student_score(struct student* stu, double score){
    assert(stu);
    stu->score = score;
}

int get_student_id(struct student* stu){
    assert(stu);
    return stu->id;
}

double get_student_score(struct student* stu){
    assert(stu);
    return stu->score;
}

int main(void)
{
    struct student boy;

    set_student_id(&boy, 20150069);
    set_student_score(&boy, 87.5);

    printf("student id: %d   \n", get_student_id(&boy));
    printf("student score: %f\n", get_student_score(&boy));

    return 0;
}
#endif // 0

// C++ Code
#include <iostream>

using std::cout;
using std::endl;

// C++中一个类对象本质上也是对应了一块内存 这里的函数是在编译期进行处理的 实际上每个成员函数都会有一个this指针 指向具体的对象
class student{
public:
    void set_student_id(/*struct student* stu*/ int student_id){
        this->id = student_id;
    }

    void set_student_score(/*struct student* stu*/  double score){
        this->score = score;
    }

    int get_student_id(/*struct student* stu*/){
        return this->id;
    }

    double get_student_score(/*struct student* stu*/){
        return this->score;
    }
private:
    int    id;
    double score;
};

int main()
{
    student stu;
    stu.set_student_id(20150069);
    stu.set_student_score(87.5);

    cout << "student id:    " << stu.get_student_id() << endl;
    cout << "student score: " << stu.get_student_score() << endl;

    return 0;
}