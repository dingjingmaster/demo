## vala
> vala 是GNOME使用现代语言特性，提供了对GLib和GObject特性的使用

### vala 是什么

1. 静态类型编程语言
2. 应用程序在运行时候唯一支持的是标准的 GLib 和 GObject 库（通过VAPI文件来描述接口并使用Vala中的系统库）
3. vala 提供了与 DBus 轻松集成

### vala 源码

- vala 源码分为两类：
 1. 以 `.vala` 结尾的源代码，这里包含了完整的 `.vala` 源码
 2. 以 `.vapi` 结尾的文件，这里记录对库的描述，既不可以用 `vala` 也不可以用 `c` 来写,它是不可编译的，仅仅在编译`vala` 时候有用

- vala 文件命名
  正常来说并没有严格要求`vala`源文件的命名。`VAPI` 文件的命名通常与它们所关联的库的 `pkg-config` 名称匹配，约定如下：
  1. vala 源文件通常包含一个主要的公共类，名字为 `命名空间` + `-` + `类名小写`
  2. 命名空间 - `NameSpaceName`
  3. 类名 - `ClassName`
  4. 方法名 - `method_name`
  5. 静态变量、枚举 - `CONSTANT_NAME`

### vala 语法
  vala 语法类似 `C#`
  合法标识符必须是：`数字` `字母` `下划线`，数字不打头。也可使用语言关键字为标识符，必须以`@`打头。

### GType 和 GObject
  vala 使用  GType 的运行时类型系统，GType为vala提供了一个名为GObject的强大对象模型。

### 内存管理
  vala 自动使用 GLib 中的内存管理系统，这是一个引用计数系统。因为没有垃圾回收器，循环引用可能导致内存泄漏。
  vala 有指针类型

### vala 编译
  vala 程序和库在编译成机器码之前被翻译成 `c` 语言。这一阶段是完全透明的，一个vala类转为一个 c 的头文件和源文件

### 程序入口
  程序入口一定是 `main` 函数
  ```vala
  void main () { ... }
  int main () { ... }
  void main (string[] args) { ... }
  int main (string[] args) { ... }
  ```
  入口点可以是隐式的，因为您可以直接在 main 函数外部的头文件中编写主代码块

## 概念

### 变量、字段、参数
  - 类型的实例以不同的方式创建，具体取决于类型。例如，基本类型是用文字表达式实例化的，而类类型是用new操作符实例化的。
  - 为了访问数据，必须以某种方式(如通过名称)标识实例。在Vala中，大致有三种方法可以实现，语义相似但不相同。

#### 变量
  在方法的可执行代码中，可以将实例赋值给变量。变量有一个名称，并且被声明为引用特定数据类型的实例。一个典型的变量声明是:
  ```vala
  int a;
  ```
  如果一个类型在变量声明语句中直接实例化，那么该变量将拥有该新实例而被创建，例如:
  ```vala
  string s = "hello,world";
  ```

#### 字段
  字段类似于变量，只是定义它的范围不同。字段可以在名称空间、类和结构中定义。对于类和结构，它们可以在类或结构的作用域内，也可以在类或结构的每个实例的作用域内。
  只要一个字段的作用域仍然存在，它就有效——对于非实例字段，这是应用程序的整个生命周期;例如实例字段，这是实例的生命周期。
  如果一个类型在字段的声明中直接实例化，那么该字段将创建为拥有该新实例的字段。

#### 参数
  传递给方法的实例可以通过方法参数列表中给出的名称在该方法中访问。
  它们的行为就像变量一样，除非默认情况下，它们不能获得分配给它们的第一个实例的所有权，即传递给方法的实例。这种行为可以通过明确的所有权转移来改变。当对参数重新赋值时，结果取决于参数的方向。假设参数没有方向修饰符，它随后将完全充当变量。

### 面向对象编程
  - Vala主要是一种面向对象的语言。本文档不打算详细描述面向对象编程，但为了使其他部分更有意义，需要解释一些事情。

  - Vala中的类是潜在多态类型的定义。多态类型是一种可以被视为多个类型的类型。其基本方法是继承，因此可以将一个类型定义为另一个类型的专门化版本。子类型的实例来自于特定的超类型，具有该超类型的所有属性，可以在需要该超类型实例的任何地方使用。

  - 无论何时需要SuperType实例，都可以使用SubType实例。这是压缩类所允许的继承范围，但是完整类的特性更强。所有非紧凑类型的类都可以有虚方法，也可以实现接口。

    ```vala
      class SuperType {
      		public int act() {
      			return 1;
            }
      }
      class SubType : SuperType {
      }
    
      // 调用
      SubType sub = new SubType();
      SuperType super = sub;
    
      sub.act();
      super.act();
    ```

    

  这里，当SubType .act()被调用时，被调用的方法将是SubType的“act”方法。调用super.act()将调用SuperType的“act”。如果act方法是虚的，则SubType 在这两种情况下都将调用Act方法。

### 引用与所有权
  - Vala中的类型实例在很大程度上是自动管理的。这意味着分配内存来存储数据，然后在不再需要数据时释放内存。然而，Vala没有运行时垃圾收集器，相反，它在编译时应用规则，在运行时可预见地释放内存。
  - Vala的内存管理系统的核心概念是所有权。实例被认为是仍在使用,只要至少有一个的访问方式,即有一些领域,指的是实例变量或参数——这样的一个标识符将被视为实例的所有者,因此实例的内存将不会被收回。当不再有任何方式访问数据实例时，它被认为是无主的，其内存将被释放。

#### 引用与所有权
  - 使用引用类型，多个标识符可以引用相同的数据实例。并非所有引用类型实例的标识符都能够拥有该实例，原因将在后面解释。因此，在编写Vala代码时，经常需要考虑实例所有权。
  - 大多数引用类型都支持引用计数。这意味着实例在内部维护当前存在多少对它的引用的计数。此计数用于决定实例是否仍在使用中，或者是否可以释放其内存。因此，以这种方式计算的每个引用都是实例的潜在所有者，因为它确保实例继续存在。有些情况下不希望这样，因此可以将字段或变量定义为“weak”。在这种情况下，引用不会被计数，因此引用存在的事实不会阻止实例被释放，也就是说，这种引用不能获得实例的所有权。
  - 当使用引用计数类型时，弱引用的主要用途是防止引用循环。当数据实例内部包含对另一个实例的引用时，就会存在这些引用，而另一个实例又包含对第一个实例的引用。在这种情况下，不可能重新分配实例，因为每个实例都可能拥有另一个实例。通过确保其中一个引用是弱引用，其中一个实例可以成为无主实例并被释放，在此过程中另一个实例将被解除引用，也可能成为无主实例并被释放。
  - 它也可以有引用类型没有引用计数;这方面的一个例子是基本字符串类型，其他是紧凑的类类型。如果Vala允许多个引用拥有这些实例，它将无法跟踪它们何时全部停止存在，因此将无法知道何时释放实例。相反，只有一个或零个标识符将拥有实例——当它为零时，实例将被释放。这意味着，对一个已经拥有的实例的所有引用要么是弱引用，要么必须使用所有权转移操作符明确地将所有权传递给新引用

#### 指针类型
  指针类型非常重要。指针类型是值类型，其实例是对其他一些数据实例的引用。因此，它们不是实际的引用，并且永远不会拥有它们间接引用的实例。

### 类型

简单来说，“类型”只是 0 个或多个数据字段的抽象集合。类型可以通过创建一个实体来实例化，该实体包含映射到该类型字段的值。在vala中，类型通常包括：

- 类型名，在Vala代码的各种上下文中使用它来表示类型的实例。
- 一种数据结构，定义如何在内存中表示该类型的实例。
- 可在该类型的实例上调用的一组方法。

这些元素作为类型的定义组合在一起。Vala的定义是以声明的形式给出的，例如类声明。

Vala支持三种数据类型:值类型、引用类型和元类型。值类型包括简单类型(例如char、int和float)、枚举类型和结构类型。引用类型包括对象类型、数组类型、委托类型和错误类型。类型参数是泛型类型中使用的参数。

值类型不同于引用类型，因为只有一个变量或字段引用每个实例，而引用类型的变量或字段存储对数据的引用，这些数据也可以被其他变量或字段引用。当引用类型的两个变量或字段引用相同的数据时，使用一个标识符所做的更改在使用另一个标识符时是可见的。对于值类型，这是不可能的。

元类型是从其他类型自动创建的，因此可能具有引用或值类型语义。

```vala
type:
	value-type
	reference-type
	meta-type

meta-type:
	parameterised-type
	nullable-type
	pointer-type
```

#### 值类型

值类型的实例直接存储在表示它们的变量或字段中。当值类型实例被分配给另一个变量或字段时，默认操作是复制该值，这样每个标识符都引用其拥有的数据的唯一副本。当值类型在方法中实例化时，实例将在堆栈上创建。

```vala
value-type:
	fundamental-struct-type
	user-defined-struct-type
	enumerated-type

fundamental-struct-type:
	integral-type
	floating-point-type
	bool

integral-type:
	char
	uchar
	short
	ushort
	int
	uint
	long
	ulong
	size_t
	ssize_t
	int8
	uint8
	int16
	uint16
	int32
	uint32
	int64
	uint64
	unichar

floating-point-type:
	float
	double
```

在指示文字的地方，这意味着给出了内置结构类型的实际类型名。Vala中包含了这些类型的定义，因此这些类型总是可用的。

#### 结构体类型

结构类型仅提供数据结构和作用于它的一些方法。结构不是多态的，不能具有诸如信号或属性之类的高级特性。关于如何定义结构和关于它们的更多细节，请参阅Structs文档。请参阅表达式/结构的实例化如何实例化结构。

结构类型实例被分配给的每个变量或字段都获得数据的一个副本，它拥有该副本的所有权。但是，当将结构类型实例传递给方法时，不会生成副本。而是传递对实例的引用。这种行为可以通过将结构声明为简单类型来改变。

#### 基本类型

在Vala中，基本类型被定义为Vala内部知道其数据结构的结构类型。它们有一个匿名字段，需要时自动访问该字段。所有基本值类型都定义为简单类型，因此无论何时将实例赋值给变量或字段，或作为函数参数传递，都将生成数据的副本。

基本值类型分为三类:布尔类型、整型类型和浮点类型。

#### 整数类型

整型只能包含整数。它们可以是有符号的，也可以是无符号的，每种类型都被认为是不同的类型，尽管在需要时可以在它们之间进行强制转换。

有些类型定义了准确地使用多少位存储来表示整数，其他类型取决于环境。long, int short映射到C数据类型，因此依赖于机器架构。Char是1字节。unichar是4字节，也就是足够大，可以存储任何UTF-8字符。

所有这些类型都可以使用文字表达式进行实例化

#### 浮点类型

浮点类型包含固定位数的实浮点数(参见IEEE 754)。所有这些类型都可以使用文字表达式进行实例化

#### bool 类型

可以有真或假的值。尽管bool实例只能接受两个值，但这不是枚举类型。每个实例都是惟一的，并将在需要时进行复制，这与其他基本值类型相同。

#### 枚举类型

枚举类型是一种类型实例可以保存的所有可能值都用该类型声明的类型。在Vala中，枚举类型是真实类型，不会隐式转换。可以在枚举类型之间显式强制转换，但通常不建议这样做。在用Vala编写新代码时，不要依赖能够以这种方式进行强制转换。

枚举类型的变体是标志类型。这表示一组标志，任何数量的标志都可以组合在一个标志类型的实例中，就像C中的位域一样。

#### 引用类型

引用类型的实例总是存储在堆上。引用类型的变量包含对实例的引用，而不是实例本身。将引用类型的实例赋值给变量或字段不会复制数据，而是只复制对数据的引用。这意味着两个变量将引用相同的数据，因此使用其中一个引用对该数据所做的更改将在使用另一个引用时可见。

任何引用类型的实例都可以被赋值一个声明为“弱”的变量。这意味着类型实例不能知道该变量。引用计数类型在分配给弱变量后不会增加它的引用计数:弱变量不能获得实例的所有权。

```vala
reference-type:
	classed-type
	array-type
	delegate-type
	error-type
	string

classed-type:
	simple-classed-type
	type-instance-classed-type
	object-classed-type

simple-classed-type:
	user-defined-simple-classed-type

type-instance-classed-type:
	user-defined-type-instance-classed-type

object-classed-type:
	user-defined-object-classed-type

array-type:
	non-array-type []
	non-array-type [ dimension-separators ]

non-array-type:
	value-type
	classed-type
	delegate-type
	error-type

dimension-separators:
	,
	dimension-separators ,

delegate-type:
	user-defined-delegate-type

error-type:
	user-defined-error-type
```

#### 类类型

类定义引入了一个新的引用类型——这是在Vala中创建新类型的最常见方法。类是一种非常强大的机制，因为它们具有多态性和继承等特性。

Vala中的大多数类类型都是引用计数的。这意味着每次将一个类类型实例分配给一个变量或字段时，不仅复制了引用，而且该实例还记录了对它的另一个引用已经创建。当字段或变量超出范围时，对该实例的引用也会被记录下来。这意味着当不再需要一个类类型实例时，可以从内存中自动删除它。唯一没有引用计数的类类型是紧凑类。内存管理在概述/内存管理中讨论。如果实例不是引用计数类型，则必须使用#操作符显式转移所有权——这将导致原始变量失效。当将一个类类型实例传递给一个方法时，应用相同的规则。可用的类的类型在类/类的类型中讨论。

#### 数组类型

数组是一种数据结构，它可以包含零个或多个相同类型的元素，最多不超过该类型所定义的限制。一个数组可以有多个维度;对于每一个可能的维数集合，都会隐含一个新的类型，但是有一个元类型可用来描述具有相同维数的任意大小的数组，即int[1]与int[2]不是同一类型，而int[]与两者都是相同的类型。

可以使用length成员从数组中获取size，如果数组只有一个维，则返回int;如果数组包含多个维，则返回int[]。

您还可以分别使用move和copy成员移动或复制和数组。

对于一维数组，resize成员也可用于更改数组的长度。

#### 代理（委托）类型

委托是引用方法的数据结构。方法在同样存储的给定范围内执行，这意味着对于实例方法来说，委托也将包含对该实例的引用。

从技术上讲，委托是一种引用类型，但由于方法是不可变的，因此这种区别不如其他类型重要。将委托分配给变量或字段不能复制所指示的方法，而且任何委托都不能以任何方式更改该方法。

#### 错误类型

错误类型的实例表示可恢复的运行时错误。所有错误都使用错误域(枚举值的一种类型)来描述，但是错误本身不是枚举类型。本文档的几个部分将详细讨论错误

#### 字符串

Vala通过基本的字符串类型内置了对Unicode字符串的支持。这是唯一的引用类型的基本类型。像其他基本类型一样，它可以用文字表达式(表达式/文字表达式)实例化。字符串是UTF-8编码的，与Vala源文件一样，这意味着它们不能像C中的字符数组那样被访问——不能保证每个Unicode字符只存储在一个字节中。相反，string基本结构类型(所有字符串都是其实例)提供了访问方法以及其他工具。

虽然字符串在技术上是一种引用类型，但它们具有与结构相同的默认复制语义——无论何时将字符串值分配给变量或字段，数据都会被复制，但只有引用作为参数传递给方法。这是必需的，因为字符串没有引用计数，所以变量或字段能够获得字符串所有权的唯一方法是分配一个字符串的副本。为了避免这种行为，可以将字符串值赋值给弱引用(在这种情况下不会复制)。

#### 泛型

Vala允许定义可以在运行时使用类型参数自定义的类型。例如，可以定义一个列表，以便将它实例化为一个int列表、一个object列表等等。这是通过使用泛型声明实现的。

#### 空类型

类型的名称可用于隐式创建与该类型相关的可空类型。一个可空类型T的实例?可以是类型为T或null的值。空类型将具有值或引用类型语义，这取决于它所基于的类型。

#### 指针类型

类型的名称可用于隐式创建与该类型相关的指针类型。声明为T类型的变量的值表示T类型实例的内存地址。实例永远不会知道它的地址被记录了，因此不能记录它被这样引用的事实。

任何类型的实例都可以赋值给一个变量，该变量被声明为指向该类型实例的指针。对于引用类型，在任何方向上都允许直接赋值。对于值类型，需要使用指针指向操作符“&”来赋值给指针，使用指针间接操作符“*”来访问所指向的实例。看到表达式/指针表达式。

void*类型表示一个指向未知类型的指针。由于引用类型未知，间接操作符不能应用于void*类型的指针，也不能在这样的指针上执行任何算术操作。然而，void*类型的指针可以转换为任何其他类型的指针(反之亦然)，并与其他指针类型的值进行比较。看到表情/类型操作。指针类型本身具有值类型语义。

#### 类型转换

如果Vala中可能存在两种类型转换，即隐式转换和显式转换。在表达式中，Vala通常会转换基本类型，以使计算成为可能。当默认转换不是您所需要的时，您可以显式强制转换，以便所有操作数都是兼容类型。有关自动转换的详细信息，请参见表达式。

Vala还将自动执行与多态性相关的转换，其中所需的强制转换是明确的，可以从上下文推断出来。这允许您在需要类类型的任何超类或实现接口的实例时使用类类型实例。Vala永远不会自动转换为子类型，因为这必须显式地完成。请参阅概念/面向对象编程，请参阅类。

### 表达式

表达式是一段简短的代码，它定义了在程序执行期间到达表达式时应该采取的操作。这样的操作可以是算术操作、调用方法、实例化类型等等。所有表达式的计算结果都是特定类型的单个值—然后可以在另一个表达式中使用该值，方法是将表达式组合在一起，或将该值分配给一个标识符。

当表达式组合在一起时(例如，将两个数字相加，然后将结果乘以另一个数字:5 + 4 * 3)，子表达式的计算顺序就变得重要了。括号用于标记哪些表达式应该嵌套在其他表达式中，例如(5 + 4)* 3表示加法表达式嵌套在乘法表达式中，因此必须先求值。

当标识符在表达式中使用时，它们计算其值，除非在赋值中使用。赋值的左手边是表达式的一种特殊情况，其中标识符本身不被认为是表达式，因此不被计算。一些操作将赋值与另一种操作(例如，递增操作)结合在一起，在这种情况下，标识符最初可以被认为是一个表达式，然后在整个表达式求值后只是一个用于赋值的标识符。

```vala
primary-expression:
	literal
	template
	member-access-expression
	pointer-member-access-expression
	element-access-expression
	postfix-expression
	class-instantiation-expression
	array-instantiation-expression
	struct-instantiation-expression
	invocation-expression
	sizeof-expression
	typeof-expression

unary-expression:
	primary-expression
	sign-expression
	logical-not-expression
	bitwise-not-expression
	prefix-expression
	ownership-transfer-expression
	cast-expression
	pointer-expression

expression:
	conditional-expression
	assignment-expression
	lambda-expression
```

#### 文字表达方式

每个字面值表达式用给定的值实例化其各自的类型。

#### 成员访问

访问另一个作用域的成员。

```vala
member-access-expression:
	[ primary-expression . ] identifier
```

如果没有提供内部表达式，则将从当前范围(例如方法中的局部变量)开始查找标识符。否则，将使用内部表达式的作用域。实例方法中的特殊标识符this(没有内部表达式)将引用类型符号(class、struct、enum等)的实例。

#### 元素访问

```vala
element-access-expression:
	container [ indexes ]

container:
	expression

indexes:
	expression [ , indexes ]
```

元素访问可用于:

- 在给定索引处访问容器的元素
- 在给定索引处将元素赋值给容器。在这种情况下，元素访问表达式是赋值操作的左边。

元素访问可用于具有get和/或set方法的字符串、数组和类型。

- 对于只能访问字符的字符串，不可能给元素赋值。
- 在数组中，既可以访问元素，也可以赋值给元素。元素访问表达式的类型与数组元素类型相同。

元素访问也可以与复杂类型(如类、结构等)一起作为容器使用:

- 如果存在一个接受至少一个参数并返回一个值的get方法，那么索引将用作参数，返回值将用作元素。
- 如果存在一个接受至少两个参数并返回void的set方法，那么索引将被用作参数，并将赋值作为最后一个参数。

### 语句

### 命名空间

### 方法

### 代理(委托)

### 错误

### 类

### 接口

### 泛型

### 结构体

### 枚举类型

### 属性

## 预处理器

  Vala预处理器是Vala的一个特殊部分，它只在语法级别起作用，允许您根据特定的编译时条件有条件地编写软件的各个部分。预处理器指令永远不会在结果代码中生成。

### 指令语法
  所有预处理指令都以(#)开头，除了文件的第一行的#!(用于Vala脚本)。
  ```vala
  vala-code:
	[ any vala code ] [ pp-condition ] [ any vala code ]

  pp-condition:
	#if pp-expression vala-code [ pp-elif ] [ pp-else ] #endif

  pp-elif:
	#elif pp-expression vala-code [ pp-elif ]

  pp-else:
	#else vala-code

  pp-expression:
	pp-or-expression

  pp-or-expression:
	pp-and-expression [ || pp-and-expression ]

  pp-and-expression:
	pp-binary-expression [ && pp-binary-expression ]

  pp-binary-expression:
	pp-equality-expression
	pp-inequality-expression

  pp-equality-expression:
	pp-unary-expression [ == pp-unary-expression ]

  pp-inequality-expression:
	pp-unary-expression [ != pp-unary-expression ]

  pp-unary-expression:
	pp-negation-expression
	pp-primary-expression

  pp-negation-expression:
	! pp-unary-expression

  pp-primary-expression:
	pp-symbol
	( pp-expression )
	true
	false

  pp-symbol:
	identifier
  ```
  预处理器的语义非常简单:如果条件为真，那么被预处理器包围的Vala代码将被解析，否则将被忽略。如果符号是在编译时定义的，则该符号的计算结果为true。如果预处理指令中的符号没有定义，它的计算结果为false。

### 定义符号
  在Vala代码中定义预处理器符号是不可能的(像C一样)。定义符号的唯一方法是通过valac选项 `-D` 提供它。

  - 内置定义

    | **Name**  | **Description**                                           |
    | --------- | --------------------------------------------------------- |
    | POSIX     | Set if the profile is posix                               |
    | GOBJECT   | Set if the profile is gobject                             |
    | DOVA      | Set if the profile is dova                                |
    | VALA_X_Y  | Set if Vala API version is equal or higher to version X.Y |
    | DBUS_GLIB | Set if using dbus-glib-1 package                          |

  - 例子
    如何基于valac选项-D有条件地编译代码。
    
    ```vala
    // Vala preprocessor example
    public class Preprocessor : Object {
    
        public Preprocessor () {
        }
    
        /* public instance method */
        public void run () {
    #if PREPROCESSOR_DEBUG
        // Use "-D PREPROCESSOR_DEBUG" to run this code path
        stdout.printf ("debug version \n");
    #else
        // Normally, we run this code path
        stdout.printf ("production version \n");
    #endif
        }
    
        /* application entry point */
        public static int main (string[] args) {
            var sample = new Preprocessor ();
            sample.run ();
            return 0;
        }
    }
    ```
    正常编译:
    ```shell
    valac -o preprocessor Preprocessor.vala
    ```
    debug编译:
    ```shell
    valac -D PREPROCESSOR_DEBUG -o preprocessor-debug Preprocessor.vala
    ```

## GIDL 元数据格式
  本节描述了.metadata文件的格式，vapigen将其用作生成.vapi文件的附加信息。元数据中指定的一些信息还可以用于设置符号属性。

- 注释: 注释以#开头，以行尾结束。例如:
   ```shell
   # this is a comment
   ```

- 其它行:
  1. 文件中的每一个非注释行都由两部分组成:说明符和参数。
  2. 说明符是该行中出现的第一个文本，它指定了该行其余部分将修改的内容。
  3. 参数是一个以空格分隔的参数名称列表，后跟等号和用引号括起来的值。
  例如，这一行设置foo.bar上的parameter1和parameter2:
  ```c
  foo.bar parameter1="value" parameter2="value"
  ```

- 说明符：
  1. 说明符总是对您正在修改的内容使用C名称。例如，如果您的名称空间是Foo，而类型的Vala名称是Bar，那么您将使用FooBar。
  2. 说明符也可以使用通配符，所有部分匹配说明符的项都将被选中。例如:
    ```shell
    *.klass hidden="1"
    ```
    将隐藏所有类型中类字段

- 指定不同的事情

   | Function             | `name_of_function`        |
   | -------------------- | ------------------------- |
   | Type                 | `Type`                    |
   | Property             | `Type:property_name`      |
   | Signal               | `Type::signal_name`       |
   | Field                | `Type.field_name`         |
   | Parameter (Function) | `name_of_function.param`  |
   | Parameter (Delegate) | `DelegateName.param`      |
   | Parameter (Signal)   | `Type::signal_name.param` |

   例如，隐藏一个符号

   | Type     | `Foo hidden="1"`           |
   | -------- | -------------------------- |
   | Function | `some_function hidden="1"` |
   | Field    | `Foo.bar hidden="1"`       |

- 属性引用

   条目的格式如下所示

   | **Name** | **Applies To**                       | **Values**            | **Description**            |
   | -------- | ------------------------------------ | --------------------- | -------------------------- |
   | `foobar` | Signal, Function, Class, Struct, etc | The acceptable values | The description goes here. |

   并按字母顺序排列：

   | **Name**                | **Applies To**                                               | **Values**                                            | **Description**                                              |
   | ----------------------- | ------------------------------------------------------------ | ----------------------------------------------------- | ------------------------------------------------------------ |
   | `abstract`              | Class, Function                                              | 0, 1                                                  |                                                              |
   | `accessor_method`       | Property                                                     | 0, 1                                                  |                                                              |
   | `array_length_cname`    | Field                                                        | C identifier                                          |                                                              |
   | `array_length_pos`      | Parameter (Function)                                         | Double (position between two Vala parameters)         | Sets the position of the length for the parameter, length needs to be hidden separately. |
   | `array_length_type`     | Parameter (Function), Function (returning an array), Field   | C type                                                |                                                              |
   | `array_null_terminated` | Function (returning an array), Parameter (Function), Field   | 0, 1                                                  |                                                              |
   | `async`                 | Function                                                     | 0, 1                                                  | Force async function, even if it doesn't end in _async       |
   | `base_class`            | Class                                                        | C type                                                | Marks the base class for the type                            |
   | `base_type`             | Struct                                                       | Vala type                                             | Marks the struct as inheriting                               |
   | `cheader_filename`      | Anything (except parameters)                                 | Header include path                                   | Compiler will adds the specified header when thing is used.  |
   | `common_prefix`         | Enum                                                         | String                                                | Removes a common prefix from enumeration values              |
   | `const_cname`           | Class (non-GObject)                                          | C type                                                |                                                              |
   | `copy_function`         | Class (non-GObject)                                          | C function name                                       |                                                              |
   | `cprefix`               | Module                                                       | String                                                |                                                              |
   | `ctype`                 | Parameter (Function), Field                                  | C type                                                |                                                              |
   | `default_value`         | Parameter (Function)                                         | Any Vala value that would be valid for the type       | Sets the default value for a parameter.                      |
   | `delegate_target_pos`   | Parameter (Function)                                         | Double (position between two Vala parameters)         |                                                              |
   | `deprecated`            | Anything (except parameters)                                 | 0, 1                                                  | Marks the thing as deprecated                                |
   | `deprecated_since`      | Anything (except parameters)                                 | Version                                               | Marks the thing as deprecated                                |
   | `ellipsis`              | Function                                                     | 0, 1                                                  | Marks that the function has a variable argument list         |
   | `errordomain`           | Enum                                                         | 0, 1                                                  | Marks the enumeration as a GError domain                     |
   | `finish_name`           | Function                                                     | C function name                                       | Sets custom asynchronous finish function                     |
   | `free_function`         | Class (non-GObject)                                          | C function name                                       | Sets a free function for the struct                          |
   | `gir_namespace`         | Module                                                       | String                                                |                                                              |
   | `gir_version`           | Module                                                       | Version                                               |                                                              |
   | `has_copy_function`     | Struct                                                       | 0, 1                                                  | marks the struct as having a copy function                   |
   | `has_destroy_function`  | Struct                                                       | 0, 1                                                  |                                                              |
   | `has_emitter`           | Signal                                                       | 0, 1                                                  |                                                              |
   | `has_target`            | Delegate                                                     | 0, 1                                                  |                                                              |
   | `has_type_id`           | Class, Enum, Struct                                          | 0, 1                                                  | Marks whether a GType is registered for this thing           |
   | `hidden`                | Anything                                                     | 0, 1                                                  | Causes the selected thing to not be output in the vapi file. |
   | `immutable`             | Struct                                                       | 0, 1                                                  | Marks the struct as immutable                                |
   | `instance_pos`          | Delegate                                                     | Double (Position between two Vala parameters)         |                                                              |
   | `is_array`              | Function (returning an array), Parameter, Field              | 0, 1                                                  | Marks the thing as an array                                  |
   | `is_fundamental`        | Class (non-GObject)                                          | 0, 1                                                  |                                                              |
   | `is_immutable`          | Class (non-GObject)                                          | 0, 1                                                  |                                                              |
   | `is_out`                | Parameter                                                    | 0, 1                                                  | Marks the parameter as "out"                                 |
   | `is_ref`                | Parameter                                                    | 0, 1                                                  | Marks the parameter as "ref"                                 |
   | `is_value_type`         | Struct, Union                                                | 0, 1                                                  | Marks type as a value type (aka struct)                      |
   | `lower_case_cprefix`    | Module                                                       | String                                                |                                                              |
   | `lower_case_csuffix`    | Interface                                                    | String                                                |                                                              |
   | `name`                  | Any Type, Function, Signal                                   | Vala identifier                                       | Changes the name of the thing, does not change namespace     |
   | `namespace`             | Any Type                                                     | String                                                | Changes the namespace of the thing                           |
   | `namespace_name`        | Signal Parameter                                             | String                                                | Specify the namespace of the parameter type indicated with type_name |
   | `no_array_length`       | Function (returning an array), Parameter (Function, Delegate) | 0, 1                                                  | Does not implicitly pass/return array length to/from function |
   | `nullable`              | Function (having a return value), Parameter                  | 0, 1                                                  | Marks the value as nullable                                  |
   | `owned_get`             | Property                                                     | 0, 1                                                  |                                                              |
   | `parent`                | Any module member                                            | String (Namespace)                                    | Strip namespace prefix from symbol and put it into given sub-namespace |
   | `printf_format`         | Function                                                     | 0, 1                                                  |                                                              |
   | `rank`                  | Struct                                                       | Integer                                               |                                                              |
   | `ref_function`          | Class (non-GObject)                                          | C function name                                       |                                                              |
   | `ref_function_void`     | Class (non-GObject)                                          | 0, 1                                                  |                                                              |
   | `rename_to`             | Any Type                                                     | Vala identifier                                       | Renames the type to something else, ie fooFloat to float (not exactly the same as `name`, AFAIK name changes both the vala name and the cname. rename_to adds the required code so that when the rename_to'ed type is used, the c type is used) |
   | `replacement`           | Anything (except parameters)                                 | The thing that replaces this                          | Specifies a replacement for a deprecated symbol              |
   | `sentinel`              | Function (with ellipsis)                                     | C value                                               | The sentinel value marking the end of the vararg list        |
   | `simple_type`           | Struct                                                       | 0, 1                                                  | Marks the struct as being a simple type, like int            |
   | `takes_ownership`       | Parameter (Function, Delegate)                               | 0, 1                                                  |                                                              |
   | `throws`                | Function                                                     | 0, 1                                                  | Marks that the function should use an out parameter instead of throwing an error |
   | `to_string`             | Enum                                                         | C function name                                       |                                                              |
   | `transfer_ownership`    | Function/Delegate/Signal (having a return value), Parameter (Function, Signal) | 0, 1                                                  | Transfers ownership of the value                             |
   | `type_arguments`        | Function/Delegate/Signal (having a return value), Property, Field, Parameter | Vala types, comma separated                           | Restricts the generic type of the thing                      |
   | `type_check_function`   | Class (GObject)                                              | C function/macro name                                 |                                                              |
   | `type_id`               | Struct, Class (GObject)                                      | C macro                                               |                                                              |
   | `type_name`             | Function (having a return value), Property, Parameter, Field | Vala type name                                        | Changes the type of the selected thing. Overwrites old type, so "type_name" must be before any other type modifying metadata |
   | `type_parameters`       | Delegate, Class (non-GObject)                                | Vala generic type parameters, e.g. T, comma separated |                                                              |
   | `unref_function`        | Class (non-GObject)                                          | C function name                                       |                                                              |
   | `value_owned`           | Parameter (Function)                                         | 0, 1                                                  |                                                              |
   | `vfunc_name`            | Function                                                     | C function pointer name                               |                                                              |
   | `virtual`               | Function                                                     | 0, 1                                                  |                                                              |
   | `weak`                  | Field                                                        | 0, 1                                                  | Marks the field as weak                                      |

## GIR metadata 格式

GIR格式实际上有很多用于生成绑定的信息，但它与Vala是不同的语言。因此，几乎不可能将整个.gir文件直接映射到Vala树中，因此需要元数据。另一方面，我们可能希望直接使用. GIR + .metadata，而不是生成.vapi，但是.vapi比GIR更易于理解，解析起来也更快，因此需要使用vapigen来生成.vapi。

### 定位元数据

SomeLib元数据的文件名。gir必须是SomeLib.metadata。默认情况下，Vala在.gir文件的相同目录中查找.metadata，但是可以使用——metadatadir选项指定其他目录。

### 注释

元数据中的注释与Vala代码中的语法相同:

```shell
// this is a comment
/*
 * multi-line comment
 */
```

### 语法

每个符号的元数据信息必须在不同的行上提供:

```vala
rule:
	pattern [ arguments ] [  relative-rules ] 

relative-rules:
	. pattern [ arguments ] [  relative-rules ]

pattern:
	[ # selector ] [ . pattern ]

arguments:
	[ = expression ] [ arguments ]

expression:
	null
	true
	false
	- expression
	integer-literal
	real-literal
	string-literal
	symbol

symbol:
	identifier [ . identifier ]
```

- 模式被绑定到GIR树:如果一个类FooBar包含一个方法baz_method，那么它就可以在元数据中作为FooBar.baz_method被引用。

- 选择器用于指定GIR树的特定元素名，例如FooBar。Baz_method #方法将只选择名称为Baz_method的方法元素。用于解决名称冲突。
- 给定一个名为Foo的命名空间，一个特殊的模式Foo可用于设置一般参数。
- 如果一个GIR符号匹配多个规则，那么所有的规则都将被应用:如果参数之间存在冲突，文件中最后写入的规则优先。
- 如果未提供参数的表达式，则默认将其视为true。
- 相对规则是相对于最近的前一个绝对规则。元数据必须包含至少一个绝对规则。不可能制定一个相对于另一个相对规则的规则。

### 有效的参数

| **Name**           | **Applies to**                                      | **Type** | **Description**                                              |
| ------------------ | --------------------------------------------------- | -------- | ------------------------------------------------------------ |
| skip               | all                                                 | bool     | Skip processing the symbol                                   |
| hidden             | all                                                 | bool     | Process the symbol but hide from output                      |
| type               | method, parameter, property, field, constant, alias | string   | Complete Vala type                                           |
| type_arguments     | method, parameter, property, field, constant, alias | string   | Vala type parameters for generics, separated by commas       |
| cheader_filename   | all including namespace                             | string   | C headers separated by commas                                |
| name               | all including namespace                             | string   | Vala symbol name                                             |
| owned              | parameter                                           | bool     | Whether the parameter value should be owned                  |
| unowned            | method, property, field, constant                   | bool     | Whether the symbol is unowned                                |
| parent             | all                                                 | string   | Move the symbol to the specified container symbol. If no container exists, a new namespace will be created. |
| nullable           | method, parameter, property, field, constant, alias | bool     | Whether the type is nullable or not                          |
| deprecated         | all                                                 | bool     | Whether the symbol is deprecated or not                      |
| replacement        | all                                                 | string   | Deprecation replacement, implies `deprecated=true`           |
| deprecated_since   | all                                                 | string   | Deprecated since version, implies `deprecated=true`          |
| array              | method, parameter, property, field, constant, alias | bool     | Whether the type is an array or not                          |
| array_length_idx   | parameter                                           | int      | The index of the C array length parameter                    |
| default            | parameter                                           | any      | Default expression for the parameter                         |
| out                | parameter                                           | bool     | Whether the parameter direction is out or not                |
| ref                | parameter                                           | bool     | Whether the parameter direction is ref or not                |
| vfunc_name         | method                                              | string   | Name of the C virtual function                               |
| virtual            | method, signal, property                            | bool     | Whether the symbol is virtual or not                         |
| abstract           | method, signal, property                            | bool     | Whether the symbol is abstract or not                        |
| scope              | parameter (async method)                            | string   | Scope of the delegate, in GIR terms                          |
| struct             | record (detected as boxed compact class)            | bool     | Whether the boxed type must be threaten as struct instead of compact class |
| printf_format      | method                                              | bool     | Add the [PrintfFormat] attribute to the method if true       |
| array_length_field | field (array)                                       | string   | The name of the length field                                 |
| sentinel           | method                                              | string   | C expression of the last argument for varargs                |
| closure            | parameter                                           | int      | Specifies the index of the parameter representing the user data for this callback |
| errordomain        | enumeration                                         | bool     | Whether the enumeration is an errordomain or not             |
| destroys_instance  | method                                              | bool     | Whether the instance is owned by the method                  |
| throws             | method                                              | string   | Type of exception the method throws                          |

### Overriding Types

当你有以下表达式:

```vala
typedef GList MyList;
```

其中，GList将保存整数，使用类型元数据如下:

```vala
MyList type="GLib.List<int>"
```

以上元数据将生成以下代码:

```vala
public class MyList : GLib.List<int> {
	[CCode (has_construct_function = false)]
    protected MyList ();
    public static GLib.Type get_type ();
}
```

然后你可以使用 GLib.List 或 NameSpace.MyList 

