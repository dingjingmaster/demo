/*************************************************************************
> FileName: write-read-struct.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 28 Mar 2023 08:24:41 PM CST
 ************************************************************************/
#include <stdio.h>
#include <string.h>

typedef struct _A
{
    int a;
    char b[10];
} A;

int main ()
{
    A a = {0};
    a.a = 3;
    A b = {0};

    strcpy(a.b, "mmk");

    FILE* fw = fopen ("/tmp/aa", "w+");
    fwrite(&a, sizeof(A), 1, fw);
    fclose(fw);

    FILE* fr = fopen ("/tmp/aa", "r");
    fread(&b, sizeof(A), 1, fr);
    fclose(fr);

    printf ("%d\n%s\n", b.a, b.b);
    

}
