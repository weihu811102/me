#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

// 符号类型表示
typedef enum {
    BAD_TOKEN,			// 错误的符号
    NUMBER_TOKEN,		// 数字符号

    ADD_OPERATOR_TOKEN, // +
    SUB_OPERATOR_TOKEN, // -
    MUL_OPERATOR_TOKEN, // *
    DIV_OPERATOR_TOKEN, // /

    END_OF_LINE_TOKEN   // 符号的结束标志
} TokenKind;

#define MAX_TOKEN_SIZE (100)

typedef struct {
    TokenKind   kind;
    double      value; 
    char        str[MAX_TOKEN_SIZE];
} Token;

void set_line(char *line);    // 读入行 这里以换行符作为结束解析的
void get_token(Token *token); // 分割token并且在行尾加上END_OF_LINE_TOKEN标记 将分割好的token放在Token结构体上

#endif /* TOKEN_H_INCLUDED */
