#include <stdlib.h>
#include <stdio.h>

#define RESET         "\x1b[0m"
#define BOLD          "\x1b[1m"
#define DIM           "\x1b[2m"
#define ITALIC        "\x1b[3m"
#define UNDERLINE     "\x1b[4m"
#define BLINK         "\x1b[5m"
#define REVERSE       "\x1b[7m"
#define HIDDEN        "\x1b[8m"
#define FG_COLOR(n)   "\x1b[38;5;" #n "m"
#define BG_COLOR(n)   "\x1b[48;5;" #n "m"


void RaiseError(
    char *Type,
    char *Description
) {
    printf(BOLD BG_COLOR(1)"Type" RESET FG_COLOR(1) " : %s"RESET"\n" , Type);
    printf(BOLD BG_COLOR(1)"Discription" RESET FG_COLOR(1) " : %s"RESET"\n" , Description);
    exit(1);
}

void RaiseErrorAtLine(
    char *Type,
    int Line,
    char *Literal,
    char *Description
) {
    printf(BOLD BG_COLOR(1)"Error at line" RESET FG_COLOR(1) " : %d"RESET"\n" , Line);
    printf(BOLD BG_COLOR(1)"Type" RESET FG_COLOR(1) " : %s"RESET"\n" , Type);
    printf(BOLD BG_COLOR(1)"Content" RESET FG_COLOR(1) " : \"%s\""RESET"\n" , Literal);
    printf(BOLD BG_COLOR(1)"Discription" RESET FG_COLOR(1) " : %s"RESET"\n" , Description);
    exit(1);
}