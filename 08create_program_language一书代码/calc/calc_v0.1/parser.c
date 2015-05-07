// �﷨��������ʵ��
// �ݹ��½���������
//		1.ɨ��������ַ��� parse_expression()���� 
//      2.parse_term()���� ɨ���ַ��� ��������ֱ�ʾ�·���st_look_ahead_token�� �����*����/���˵õ� �ո�ɨ���v1����
//      3.parse_term()��������(����+����-) ���ʱ��õ�v2���� v1+v2 ���� v1-v2 

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

#define LINE_BUF_SIZE (1024)

static Token st_look_ahead_token;        // ��ǻ��˵�token ��Ϊ����ʹ�õ��ǵݹ��½������� ������Ҫ����
static int   st_look_ahead_token_exists; // �Ƿ���ڵ�token

static void
my_get_token(Token *token)
{
    if (st_look_ahead_token_exists) {
        *token = st_look_ahead_token;
        st_look_ahead_token_exists = 0;
    } else {
        get_token(token); // ���û�б����ǰ��token
    }
}

// ��ǻ��˵�token
static void
unget_token(Token *token)
{
    st_look_ahead_token = *token;
    st_look_ahead_token_exists = 1;
}

double parse_expression(void);

// һԪ���ʽ�Ľ���
static double
parse_primary_expression()
{
    Token token;

    my_get_token(&token); // �õ�һ��token����
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
            unget_token(&token); // ���token���� *����/ ������token break
            break;
        }

        v2 = parse_primary_expression(); // v2
        if (token.kind == MUL_OPERATOR_TOKEN) { // * ���� *����/�õ����
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
        if (token.kind != ADD_OPERATOR_TOKEN && token.kind != SUB_OPERATOR_TOKEN) { // + - ���� +����-�Ļ� ������token����ɨ��
            unget_token(&token);
            break;
        }

        v2 = parse_term(); // v2
        if (token.kind == ADD_OPERATOR_TOKEN) { // + ����*����-
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

    st_look_ahead_token_exists = 0; // ���˵�
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