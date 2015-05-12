#include <iostream>	
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

void str_trim_crlf(char *str);
void str_trim_comma(char *str);
int str_all_space(const char *str);

vector<string> v_; // 用一个vector来保存屏蔽字

// TODO  :  使用这个函数来判断输入的字符是否在屏蔽字内
// RET 0 :  在屏蔽字内
//     -1:  不在屏蔽字内
int isHaveThisWord(const string& word) 
{
	vector<string>::iterator iter = v_.begin();
	for(; iter != v_.end(); ++iter)
	{
		if(*iter == word)
		{
			return 0;
		}
	}
	if(iter ==  v_.end()) // 没找到相等的 不在屏蔽字内
	{
		return -1;
	}
}
void loadMaskWordFile(const char* path)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL)
		return;
	
	char line[1024] = {0};
	while (fgets(line, sizeof(line), fp) != NULL)            // 一行一行读
	{
		if (strlen(line) == 0 || str_all_space(line)) // 跳过空行
			continue;

		str_trim_crlf(line);                      // 去除 \r \n
		str_trim_comma(line);                     // 去除逗号
		v_.push_back(string(line, strlen(line))); // 这里是用\0 判断的所以用的strlen
		memset(line, 0, sizeof(line));
	}

	fclose(fp);
}
void printMaskWord()
{
	vector<string>::iterator iter = v_.begin();
	for(; iter != v_.end(); ++iter)
	{
		cout << *iter << endl;
	}
}
void str_trim_crlf(char *str)
{
    char *p = &str[strlen(str)-1];
    while (*p == '\r' || *p == '\n')
        *p-- = '\0';
}
void str_trim_comma(char *str)
{
    char *p = &str[strlen(str)-1]; // 指向最后的位置
    while (*p == ',')
        *p-- = '\0';
}
int str_all_space(const char *str)
{
    while (*str)
    {
        if (!isspace(*str))
            return 0;
        str++;
    }
    return 1;
}

// ------------------------------  使用测试 ----------------------------------------

#if 0

int main()
{
	loadMaskWordFile("MaskWord.csv");
	printMaskWord();

	string str1 = "毛泽东";
	string str2 = "阳光灿烂的日子";

	cout << " 总理           ：" << isHaveThisWord(str1) << endl;
	cout << " 阳光灿烂的日子 ：" << isHaveThisWord(str2) << endl;

	return 0;
}

#endif 