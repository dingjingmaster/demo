/*************************************************************************
> FileName: ./function.m
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 31 Mar 2023 05:57:14 PM CST
 ************************************************************************/
#include <Foundation/Foundation.h>

int func();

int main ()
{
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

   int i = 0;
   float m = 0;

   NSLog(@"value of i: %d\n", i);
   NSLog(@"value of m: %f\n", m);
   NSLog(@"function: %d\n", func());    // 注意：类型不对则不会输出 期望的值。

   [pool drain];

   return 0;
}

int func()
{
    return 99;
}
