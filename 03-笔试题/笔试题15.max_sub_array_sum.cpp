#include <iostream>
#include <cstdlib> 
#include <ctime>

using namespace std;

template<typename T>
void init(T array[], int len)
{
    srand((unsigned int)(time(NULL)));
    
    for(int i=0; i<len; i++)
    {
        array[i] = rand() % 10 - 5;     
    }
}

template<typename T>
void print_array(T array[], int len)
{
    for(int i=0; i<len; i++)
    {
        cout<<array[i]<<" ";
    }
    
    cout<<endl;
}

// 动态规划 每次做更新的时候 找到一些条件判断这次更新是否正确
// 如果部分和比当前值还小 舍弃部分和 该学习死啊算法了 不能丢弃
template<typename T>
bool max_sub_array_sum(T array[], int len, T& max_sum)
{
    int ret = (len > 1);
    
    if( ret )
    {
        T sum = array[0];
        T cur = array[0];
        
        for(int i=1; i<len; i++)
        {
            cur = cur + array[i];
            
            if( cur < array[i] )
            {
                cur = array[i];
            }
            
            if( cur > sum )
            {
                sum = cur;
            }
        }
        
        max_sum = sum;
    }
    
    return ret;
}

int main()
{
    int array[10] = {0};
    int max_sum = 0;
    
    init(array, 10);
    
    print_array(array, 10);
    
    if( max_sub_array_sum(array, 10, max_sum) )
    {
        cout<<max_sum<<endl;
    }
    
	return 0;
}

