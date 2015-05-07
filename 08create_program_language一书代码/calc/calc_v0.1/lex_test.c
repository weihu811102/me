#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

void
parse_line(char *buf)
{
    Token token;

    set_line(buf);

    for (;;) {
        get_token(&token);
        if (token.kind == END_OF_LINE_TOKEN) {
            break;
        } else {
            printf("kind : %d, str : %s\n", token.kind, token.str);
        }
    }
}

int
main(int argc, char **argv)
{
    char buf[1024];

    while (fgets(buf, 1024, stdin) != NULL) {
        parse_line(buf);
    }

    return 0;
}
