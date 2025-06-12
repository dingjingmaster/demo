/*************************************************************************
> FileName: demo12.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月12日 星期四 13时35分37秒
 ************************************************************************/
#include "demo12/demo12.h"

#include <stdio.h>


int main ()
{
    MyObject* obj = my_object_new();
    my_object_set_name (obj, "Name");
    printf ("%s\n", my_object_get_name(obj));
    return 0;
}
