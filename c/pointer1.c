
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char **p;
    char *buf = malloc(20);

    memcpy(buf, "hello, world!", 20);
    p = &buf;
    //p[1] = NULL; p's step is 4, so that p[1]=NULL
    printf("%d\n", NULL);       // 0
    //p[1] = NULL; // wrong: p is a pointer, p[1] equal *(p + 1) is a pointer


    printf("p: %x\n", p);           // 0x60fea8
    printf("p[1]: %x\n", p[1]);     // 0x60fea8

    printf("*p: %c\n", *p);                 // no value. it's the part of a pointer
    printf("**p: %c\n", **p);               // h. it's equal *buf
    printf("*(p[1]): %c\n", *(p[1]));       // no value


    *(*p + 1) = NULL; //right: --- result: h|
    while (*buf)
    printf("%c ", *(buf++));
    printf("|\n");

    return 0;
}
