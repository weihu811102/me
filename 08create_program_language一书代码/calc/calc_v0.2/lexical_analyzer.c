// 词法分析器的实现

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

static char *st_line;   // 记录解析的行
static int st_line_pos; // 记录解析的位置

// 1.开始的时候是	INITIAL_STATUS
// 2.读取到整数		IN_INT_PART_STATUS
// 3.读取到小数点	DOT_STATUS
// 3.小数点之后是   IN_FRAC_PART_STATUS

typedef enum {
    INITIAL_STATUS,     // 开始状态
    IN_INT_PART_STATUS, // 整数状态
    DOT_STATUS,			// 小数点
    IN_FRAC_PART_STATUS // 小数状态  
} LexerStatus;

// 一个字符一个字符的读取保存到token中
void
get_token(Token *token)
{
    int out_pos = 0;
    LexerStatus status = INITIAL_STATUS; // inital status
    char current_char;

    token->kind = BAD_TOKEN;
    while (st_line[st_line_pos] != '\0') {
        current_char = st_line[st_line_pos];

		// 如果当前状态是整数或者是小数并且当前字符既不是数字也不是小数点 那么标记当前字符的类型为数字
        if ((status == IN_INT_PART_STATUS || status == IN_FRAC_PART_STATUS)  
				&& !isdigit(current_char) 
				&& current_char != '.') {
            token->kind = NUMBER_TOKEN;
            sscanf(token->str, "%lf", &token->value);
            return;
        }

		// 去除空格
        if (isspace(current_char)) { 
            if (current_char == '\n') { // \r\n
                token->kind = END_OF_LINE_TOKEN;
                return;
            }
            st_line_pos++;
            continue;
        }

        if (out_pos >= MAX_TOKEN_SIZE-1) { 
            fprintf(stderr, "token too long.\n");
            exit(1);
        }

        token->str[out_pos] = st_line[st_line_pos];
        st_line_pos++;
        out_pos++;
        token->str[out_pos] = '\0';

        if (current_char == '+') {
            token->kind = ADD_OPERATOR_TOKEN;
            return;
        } else if (current_char == '-') {
            token->kind = SUB_OPERATOR_TOKEN;
            return;
        } else if (current_char == '*') {
            token->kind = MUL_OPERATOR_TOKEN;
            return;
        } else if (current_char == '/') {
            token->kind = DIV_OPERATOR_TOKEN;
            return;
	    } else if (current_char == '(') {
            token->kind = LEFT_PAREN_TOKEN;
            return;
        } else if (current_char == ')') {
            token->kind = RIGHT_PAREN_TOKEN;
            return;
        } else if (isdigit(current_char)) {
            if (status == INITIAL_STATUS) { // 开始的话是整数状态
                status = IN_INT_PART_STATUS;
            } else if (status == DOT_STATUS) { // 如果读取到小数点 那么再继续读取就是小数状态了
                status = IN_FRAC_PART_STATUS;
            }
        } else if (current_char == '.') {
            if (status == IN_INT_PART_STATUS) {
                status = DOT_STATUS;
            } else {
                fprintf(stderr, "syntax error.\n");
                exit(1);
            }
		}
    }
}

void
set_line(char *line)
{
    st_line = line;
    st_line_pos = 0;
}

