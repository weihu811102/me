#include <iostream>
using namespace std;

// 思路是从右上角开始查找 从右到左 从上到下
// 不能从左上角对角线查找 因为可能是4*2这种矩阵
// 步骤: 1.从右上角开始比较 小于该元素向左找 大于向下找  实际上类似于二分查找
template<typename T, int N, int M>
bool find_in_matrix(T matrix[N][M], T value) {// O(N + M) O(常数)
    if( (matrix[0][0] <= value) && (value <= matrix[N-1][M-1]) ){    
        int r = 0;
        int c = M - 1; // 开始位置就是右上角的位置
        
        while( (r < N) && (c >= 0) ){        
            if( value == matrix[r][c] ){            
                return true;
            }
            else if( value < matrix[r][c] ){   // 小于向左边         
                c--;
            }
            else{   // 大于向下         
                r++;
            }                
        }
    }    
    return false;
}

int main()
{
    int matrix[4][4] = 
    {
        {1, 2, 8, 9},
        {2, 4, 9, 12},
        {4, 7, 10, 13},
        {6, 8, 11, 15},
    };
    
    cout<<find_in_matrix<int, 4, 4>(matrix, 7)<<endl;    
    return 0;
}