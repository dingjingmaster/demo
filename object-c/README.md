# Objective-C

## 简介

Objective-C是通用语言，它是在C语言编程语言之上开发的，它增加了Small Talk编程语言的功能，使Objective-C成为面向对象的语言。 它主要用于开发iOS和Mac OS X操作系统及其应用程序。

### 面向对象的编程

- 封装
- 数据隐藏
- 继承
- 多态性

```objective-c
#import <Foundation/Foundation.h>

int main (int argc, const char * argv[]) {
   NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
   NSLog (@"hello world");
   [pool drain];
   return 0;
}
```

### 基础框架

Foundation Framework提供了大量函数功能，如下所示。

它包括一个扩展数据类型列表，如：NSArray，NSDictionary，NSSet等。它由一组丰富的函数组成，用于处理文件，字符串等。它提供了URL处理功能，日期格式化，数据处理，错误处理等实用程序。


