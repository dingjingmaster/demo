/*************************************************************************
> FileName: c/test.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 22 Jun 2022 02:40:44 PM CST
 ************************************************************************/
#include "test.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_JavaMain_show (JNIEnv* env, jobject thisObj)
{
    printf ("C progress:  Hello world!\n");

    return;
}
