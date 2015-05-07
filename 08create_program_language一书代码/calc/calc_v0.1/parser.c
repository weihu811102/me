// 语法分析器的实现
// 递归下降解析法：
//		1.扫描输入的字符串 parse_expression()函数 
//      2.parse_term()函数 扫描字符串 如果是数字标示下放在st_look_ahead_token中 如果是*或者/回退得到 刚刚扫描的v1计算
//      3.parse_term()函数返回(遇到+或者-) 这个时候得到v2计算 v1+v2 或者 v1-v2 

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

#define LINE_BUF_SIZE (1024)

static Token st_look_ahead_token;        // 标记回退的token 因为这里使用的是递归下降解析法 所以需要回退
static int   st_look_ahead_token_exists; // 是否存在的token

static void
my_get_token(Token *token)
{
    if (st_look_ahead_token_exists) {
        *token = st_look_ahead_token;
        st_look_ahead_token_exists = 0;
    } else {
        get_token(token); // 如果没有标记向前的token
    }
}

// 标记回退的token
static void
unget_token(Token *token)
{
    st_look_ahead_token = *token;
    st_look_ahead_token_exists = 1;
}

double parse_expression(void);

// 一元表达式的解析
static double
parse_primary_expression()
{
    Token token;

    my_get_token(&token); // 得到一个token符号
    if (token.kind == NUMBER_TOKEN) { // number
        return token.value;
    }

    fprintf(stderr, "syntax error.\n");
    exit(1);
    return 0.0; /* make compiler happy */
}

// term
static double
parse_term()
{
    double v1;
    double v2;
    Token token;

    v1 = parse_primary_expression(); // v1
    for (;;) {
        my_get_token(&token);
        if (token.kind != MUL_OPERATOR_TOKEN && token.kind != DIV_OPERATOR_TOKEN) { // * /
            unget_token(&token); // 如果token不是 *或者/ 标记这个token break
            break;
        }

        v2 = parse_primary_expression(); // v2
        if (token.kind == MUL_OPERATOR_TOKEN) { // * 计算 *或者/得到结果
            v1 *= v2;
        } else if (token.kind == DIV_OPERATOR_TOKEN) { // /
            v1 /= v2;
        }
    }
    return v1;
}

double
parse_expression()
{
    double v1;
    double v2;
    Token token;

    v1 = parse_term(); // v1
    for (;;) {
        my_get_token(&token);
        if (token.kind != ADD_OPERATOR_TOKEN && token.kind != SUB_OPERATOR_TOKEN) { // + - 不是 +或者-的话 标记这个token继续扫描
            unget_token(&token);
            break;
        }

        v2 = parse_term(); // v2
        if (token.kind == ADD_OPERATOR_TOKEN) { // + 计算*或者-
            v1 += v2;
        } else if (token.kind == SUB_OPERATOR_TOKEN) { // -
            v1 -= v2;
        } else {
            unget_token(&token);
        }
    }
    return v1;
}

double
parse_line(void)
{
    double value;

    st_look_ahead_token_exists = 0; // 回退的
    value = parse_expression();

    return value;
}

int
main(int argc, char **argv)
{
    char line[LINE_BUF_SIZE];
    double value;

    while (fgets(line, LINE_BUF_SIZE, stdin) != NULL) {
        set_line(line);
        value = parse_line();
        printf(">>%f\n", value);
    }

    return 0;
}