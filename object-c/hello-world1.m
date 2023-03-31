/*************************************************************************
> FileName: hello-world1.m
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 31 Mar 2023 04:58:10 PM CST
 ************************************************************************/
#import <Foundation/Foundation.h>       // 


// 创建接口 SampleClass 继承自 NSObject，NSObject 是所有对象的基类
@interface SampleClass:NSObject
// 声明一个方法
- (void)sampleMethod;
// 接口结束
@end

// 实现 SampleClass 接口 
@implementation SampleClass

// 实现了 sampleMethod 方法
- (void)sampleMethod 
{
    // Objective-C 中的函数，用于生成一条消息 "Hello, World!" 并显示在屏幕上
    NSLog(@"Hello, World! \n");
}

// 实现完毕
@end


// 程序入口
int main() 
{
    /**
     * autorelease 的使用方法如下：
     * 1. 生成并持有 NSAutoreleasePool 对象
     * 2. 调用已分配对象的 autorelease 实例方法
     * 3. 废弃 NSAutoreleasePool 对象
     *
     * NSAutorleasePool对象的生命周期相当于c语言变量的作用域。对于所有调用过autorelease实例方法的对象，
     * 在废弃 NSAutoreleasePool 对象都自动调用 release 实例方法。
     *
     * 示例代码：
     *  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] int];
     *  id obj = [[NSObject alloc] init];
     *  [obj autorelease];
     *  [pool drain];   // 等价于 [obj release]
     */
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

   SampleClass *sampleClass = [[SampleClass alloc]init];
   [sampleClass sampleMethod];

   [pool drain];

   // 退出
   return 0;
}


