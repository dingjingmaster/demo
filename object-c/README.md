# Object-C

## 简介

Object-C是通用语言，它是在C语言编程语言之上开发的，它增加了Small Talk编程语言的功能，使Object-C成为面向对象的语言。 它主要用于开发iOS和Mac OS X操作系统及其应用程序。

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

### 接口和实现

在Objective里完成的文件被称为界面文件，该类文件的定义被称为实现文件。

一个简单的界面文件MyClass.h将如图所示:
```object-c
@interface MyClass:NSObject{
// 类变量声明
}
// 类属性声明
// 类方法和声明
@end
```

执行MyClass.m文件，如下所示：
```object-c
@implementation MyClass
// 类方法定义
@end
```

### 创建对象

完成创建对象，如下所示：
```object-c
MyClass  *objectName = [[MyClass alloc]init];
```

### 方法
Objective C中声明的方法如下所示:
```object-c
-(returnType)methodName:(typeName) variable1 :(typeName)variable2;
```

例子：
```object-c
-(void)calculateAreaForRectangleWithLength:(CGfloat)length
andBreadth:(CGfloat)breadth;
```
你可能会想什么是andBreadth字符串，其实它的可选字符串可以帮助我们阅读和理解方法，尤其是当方法被调用的时候。

在同一类中调用此方法，我们使用下面的语句。

```object-c
[self calculateAreaForRectangleWithLength:30 andBreadth:20];
```

正如上文所说的andBreath使用有助于我们理解breath是20。Self用来指定它是一个类的方法。

### 类方法

直接而无需创建的对象，可以访问类方法。他们没有任何变量和它关联的对象。示例如下:
```object-c
+(void)simpleClassMethod;
```

它可以通过使用类名（假设作为MyClass类名称）访问，如下所示:
```object-c
[MyClass simpleClassMethod];
```

### 实例方法
可以创建的类的对象后只访问实例方法，内存分配到的实例变量。实例方法如下所示:

```object-c
-(void)simpleInstanceMethod;
```

创建类的对象后，它可以访问它。如下所示：
```object-c
MyClass  *objectName = [[MyClass alloc]init] ;
[objectName simpleInstanceMethod];
```

### Objective C的重要数据类型

|序号|数据类型|
|---|---|
|1|NSString字符串|
|2|CGfloat浮点值的基本类型|
|3|NSInteger整型|
|4|BOOL布尔类型|

### 打印日志

NSLog用于打印一份声明，它将打印在设备日志和调试版本的控制台和分别调试模式上。

```object-c
NSlog(@"");
```

### 控制结构

除了几个增补的条款外，大多数的控制结构与C以及C++相同

#### 属性

用于访问类的外部类的变量属性

#### 访问属性

可以使用点运算符访问属性，若要访问上一属性可以执行以下操作

```object-c
self.myString = @"Test";
```

还可以使用set的方法，如下所示：

```object-c
[self setMyString:@"Test"];
```

#### 类别

类用于将方法添加到现有类。通过这种方法可以将方法添加到类，甚至不用执行文件，就可以在其中定义实际的类。MyClass的样本类别，如下所示：

```object-c
@interface MyClass(customAdditions)
- (void)sampleCategoryMethod;
@end

@implementation MyClass(categoryAdditions)

-(void)sampleCategoryMethod{
   NSLog(@"Just a test category");
}
```

#### 数组
NSMutableArray 和 NSArray 是 ObjectiveC 中使用的数组类，前者是可变数组，后者是不可变数组。如下:

```object-c
NSMutableArray *aMutableArray = [[NSMutableArray alloc]init];
[anArray addObject:@"firstobject"];
NSArray *aImmutableArray = [[NSArray alloc]
initWithObjects:@"firstObject",nil];
```

#### 词典
NSMutableDictionary和NSDictionary是Objective中使用的字典，前者可变词典，后者不可变词典，如下所示:
```object-c
NSMutableDictionary*aMutableDictionary = [[NSMutableArray alloc]init];
[aMutableDictionary setObject:@"firstobject" forKey:@"aKey"];
NSDictionary*aImmutableDictionary= [[NSDictionary alloc]initWithObjects:[NSArray arrayWithObjects:
@"firstObject",nil] forKeys:[ NSArray arrayWithObjects:@"aKey"]];
```

## Object-c Linux 环境搭建

gcc-objc 是GCC编译 object-C 的前端，GNUStep可以用来模拟苹果运行环境，提供了一系列库

```shell
pacman -S gcc-objc gnustep-base gnustep-make
```

## Object-C 程序基本组成部分

- 预处理程序命令
- 接口
- 实现
- 方法
- 变量
- 声明和表达
- 注释

### Objective-C令牌

Objective-C程序是由令牌组成，令牌可以是关键字、标识符、常量、字符串、符号。例如，以下Objective-C语句由六个令牌组成：
```object-c
NSLog(@'Hello, world!\n');

// 单个标记是：
NSLog
@
(
    "Hello World!\n"
)
;                       // 分号表示终止符，每条语句必须以分号结束
```

### 关键字

|||
|----|----|
|`auto`||
|`break`||
|`case`||
|`char`||
|`const`||
|`continue`||
|`default`||
|`do`||
|`double`||
|`else`||
|`enum`||
|`extern`||
|`float`||
|`for`||
|`goto`||
|`if`||
|`int`||
|`nonatomic`||
|`protocol`||
|`property`||
|`long`||
|`readwrite`||
|`register`||
|`return`||
|`retain`||
|`readonly`||
|`short`||
|`signed`||
|`sizeof`||
|`static`||
|`struct`||
|`strong`||
|`interface`||
|`implementation`||
|`typedef`||
|`union`||
|`unsigned`||
|`unsafe_unretained`||
|`void`||
|`volatile`||
|`while`||
|`weak`||
|`CGFloat`||
|`_Packed`||
|`NSObject`||
|`NSInteger`||
|`NSNumber`||

### Objective-C空白格

只包含空格(可能带有注释)的行称为空行，而Objective-C编译器完全忽略它。

### 数据类型
