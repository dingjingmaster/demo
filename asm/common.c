/*************************************************************************
> FileName: common.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年01月05日 星期三 14时58分09秒
 ************************************************************************/
#include <stdio.h>

int get_int ()
{
    int d = 0;
    scanf ("%d", d);

    return d;
}

double get_double ()
{
    double d = 0;
    scanf ("%ld", d);

    return d;
}

void print_hello ()
{
    printf ("hello\n");
}

void print_enter ()
{
    printf ("\n");
}

void print_short (short i)
{
    printf ("%d\n", i);
}

void print_int (int i)
{
    printf ("%d\n", i);
}

void print_float (float f)
{
    printf ("%f\n", f);
}

void print_double (double f)
{
    printf ("%f\n", f);
}

void print_long (long i)
{
    printf ("%ld\n", i);
}

void print_arr_int (int arr[], int len)
{
    for (int i = 0; i < len; ++i) {
        printf ("%d  ", arr[i]);
    }

    printf ("\n");
}

void print_string (const char* s)
{
    if (!s) return;

    printf ("%s\n", s);
}
