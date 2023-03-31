/*************************************************************************
> FileName: hello-world.m
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 31 Mar 2023 04:38:04 PM CST
 ************************************************************************/
 #import <Foundation/Foundation.h>          // 预处理器

int main (int argc, const char * argv[]) 
{
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

   NSLog (@"hello world");

   [pool drain];

   return 0;
}

