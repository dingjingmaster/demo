# Rust

## cargo

- `cargo new xxx` 创建一个 `bin` 项目(如果创建`lib`项目使用 `cargo new xxx --lib`)
- `cargo run` 编译和运行项目
- `cargo run --release` 编译和运行release版本项目
- `cargo build` 编译项目(默认debug版本)
- `cargo build --release` 编译项目(默认release版本)
- `cargo check` 快速检查代码是否编译通过、比build快
- `Cargo.toml` 是cargo特有的项目数据描述文件。它存储了项目的所有元配置信息。
- `Cargo.lock` 是 cargo 工具根据同一项目的 toml 文件生成的项目依赖详细清单，不要手动修改。(一般项目是 `bin` 则上传`Cargo.lock`，`lib`项目不必上传)

## Cargo.toml


## 常量

- 常量不允许使用 `mut`。常量默认不可变、至始至终不可变。
- 常量使用 `const` 关键字而不是 `let` 关键字来声明，而且值的类型必须标注。

## 基本类型

- 数值类型：
    - 有符号整数：`i8`、`i16`、`i32`、`i64`、`i128`、 `isize`(isize取决于目标平台大小：32位是4字节，64位是8字节)
    - 无符号整数：`u8`、`u16`、`u32`、`u64`、`u128`、`usize`(usize取决于目标平台大小：32位是4字节，64位是8字节)
    - 浮点数：`f32`、`f64`(双精度, 默认)
- 字符串切片：`&str`
- 布尔类型：`true`和`false`
- 字符类型：`char`表示单个 Unicode(存储为4字节)，如`A`、`中`
- 单元类型：`()`，其唯一可能的值也是`()`

## 所有权

- Rust中每个值都被一个变量所拥有，该变量被称为值的所有者
- 一个值同时被一个变量拥有
- 当所有者(变量)离开作用域范围时候，这个值将被丢弃
- 针对 基本类型 和 针对复合类型区别

```rust
// 没有发生所有权的转移，最终 x 和 y 都等于 5
let x = 5;
let y = x;

// 发生了所有权的转移(浅拷贝 !! 移动)
let s1 = String::from("hello");
let s2 = s1;
```

> 说浅拷贝也不合理，因为 所有权转移后原始变量无法访问之前的地址了，因此说 “移动” 更合理

## 克隆(深拷贝)

```rust
let s1 = String::from("hello");
let s2 = s1.clone();

println!("s1 = {}, s2 = {}", s1, s2);
```
> 默认永远是浅拷贝
> 针对基本类型，在编译时候知道值所占存储大小，所以复制起始很快，这里不说深拷贝和浅拷贝。

## 函数传值与返回

基本类型 和 复合类型 值的归属移动
```
fn func1() {
    let s = String::from("hello");  // s 进入作用域

    takes_ownership(s);             // s 的值移动到函数里 ...
                                    // ... 所以到这里不再有效

    let x = 5;                      // x 进入作用域

    makes_copy(x);                  // x 应该移动函数里
}

fn takes_ownership(some_string: String) { // some_string 进入作用域
    println!("{}", some_string);
} // 这里，some_string 移出作用域并调用 `drop` 方法。占用的内存被释放

fn makes_copy(some_integer: i32) { // some_integer 进入作用域
    println!("{}", some_integer);
} // 这里，some_integer 移出作用域。不会有特殊操作
```

函数返回值归属权

```
fn func1() {
    let s1 = gives_ownership();         // gives_ownership 将返回值
                                        // 移给 s1

    let s2 = String::from("hello");     // s2 进入作用域

    let s3 = takes_and_gives_back(s2);  // s2 被移动到
                                        // takes_and_gives_back 中,
                                        // 它也将返回值移给 s3
} // 这里, s3 移出作用域并被丢弃。s2 也移出作用域，但已被移走，所以什么也不做。s1移除作用域并被丢弃


fn gives_ownership() -> String {             // gives_ownership 将返回值移动给
                                             // 调用它的函数

    let some_string = String::from("hello"); // some_string 进入作用域.

    some_string                              // 返回 some_string 并移出给调用的函数
}

// takes_and_gives_back 将传入字符串并返回该值
fn takes_and_gives_back(a_string: String) -> String { // a_string 进入作用域

    a_string  // 返回 a_string 并移出给调用的函数
}
```

## 引用与借用

仅通过 转移所有权 方式获取一个值，会使程序变得复杂。

Rust引入引用来使其像其它语言那样使用某个变量的指针或引用。

常规引用是一个指针类型，指向了对象存储的内存地址。

```rust
fn func1() {
    let x = 5;
    let y = &x;

    assert_eq!(5, x);
    assert_eq!(5, *y);
    // 变量 x 存放了一个 i32 值 5
    // y 是 x 的一个引用。
    // 可以断言 x 等于 5。
    // 如果希望对 y 的值作出断言，必须使用 *y 来解引用所指向的值。
}
```

## 不可变引用

```
fn func1() {
    let s1 = String::from("hello");

    let len = calculate_length(&s1);

    println!("The length of '{}' is {}.", s1, len);
}

fn calculate_length(s: &String) -> usize {
    s.len()
}
```

通过 `&s1` 鱼贩，创建了一个指向 `s1` 的引用，但是并不拥有它。因为不拥有，所以当引用离开作用域其指向的值也不会被丢弃。

## 可变引用

```
fn func1() {
    let mut s = String::from("hello");

    change(&mut s);
}

fn change(some_string: &mut String) {
    some_string.push_str(", world");
}
```

1. 声明 `s` 是可变类型
2. 创建一个可变的引用 `&mut s` 和可接受可变引用参数 `some_string: &mut String` 的函数

可变引用只能有一个

下面是错误示例：
```
let mut s = String::from("hello");

let r1 = &mut s;
let r2 = &mut s;

println!("{}, {}", r1, r2);
```
第一个可变借用 `r1` 必须要持续到最后一次使用的位置 `println!`，在 `r1` 创建和最后一次使用之间，不能创建第二个可变借用 `s2`。

下面是一个正确的示例：

```
let mut s = String::from("hello");

{
    let r1 = &mut s;

} // r1 在这里离开了作用域，所以我们完全可以创建一个新的引用

let r2 = &mut s;
```

可变引用和不可变引用不能同时存在：

```
let mut s = String::from("hello");

let r1 = &s; // 没问题
let r2 = &s; // 没问题
let r3 = &mut s; // 大问题

println!("{}, {}, and {}", r1, r2, r3);
```
- 多个不可变借用被允许是因为没有人会去试图修改数据
- 正在借用不可变引用的用户，肯定不希望他借用的东西，被另外一个人莫名其妙改变了

```
fn main() {
   let mut s = String::from("hello");

    let r1 = &s;
    let r2 = &s;
    println!("{} and {}", r1, r2);
    // 新编译器中，r1,r2作用域在这里结束

    let r3 = &mut s;
    println!("{}", r3);
} // 老编译器中，r1、r2、r3作用域在这里结束
  // 新编译器中，r3作用域在这里结束
```

## 悬垂引用(野指针)

悬垂引用也叫做悬垂指针，意思为指针指向某个值后，这个值被释放掉了，而指针仍然存在，其指向的内存可能不存在任何值或已被其它变量重新使用。

在 Rust 中编译器可以确保引用永远也不会变成悬垂状态：当你获取数据的引用后，编译器可以确保数据不会在引用结束前被释放，要想释放数据，必须先停止其引用的使用。

以下例子编译会报错：

```rust
fn main() {
    let reference_to_nothing = dangle();
}

fn dangle() -> &String {
    let s = String::from("hello");
    &s // 返回字符串 s 的引用，这里 s 离开作用域并被丢弃。其内存被释放。危险！
}
// 这里最好的解决方法就是 返回 s，最终 s 的所有权转移到外面的调用者
```

## 借用规则总结

- 同一时刻，你只能拥有要么一个可变引用，要么任意多个不可变引用
- 引用必须总是有效的

## 复合类型 -- 字符串与切片

来个简单代码，会报错：

```
fn main() {
    let my_name = "Pascal";
    greet(my_name);
}

fn greet(name: String) {
    println!("Hello, {}!", name);
}
```

greet 函数需要一个 `String` 类型的字符串，却传入了一个 `&str` 类型的字符串

### 切片

切片允许你引用集合中部分连续的元素序列，而不是引用整个集合

对于字符串而言，切片就是对 String 类型中某一部分的引用，它看起来像这样：
```
let s = String::from("hello world");

let hello = &s[0..5];
let world = &s[6..11];
```

这就是创建切片的语法，使用方括号包括的一个序列：[开始索引..终止索引]。

其中开始索引是切片中第一个元素的索引位置，而终止索引是最后一个元素后面的索引位置。换句话说，这是一个 右半开区间（或称为左闭右开区间）

以下代码等效
```
let s = String::from("hello");

let slice = &s[0..2];
let slice = &s[..2];

// 包含最后一个字节
let s = String::from("hello");
let len = s.len();
let slice = &s[4..len];
let slice = &s[4..];

// 完整截取
let s = String::from("hello");
let len = s.len();
let slice = &s[0..len];
let slice = &s[..];
```

在对字符串使用切片语法时需要格外小心，切片的索引必须落在字符之间的边界位置，也就是 UTF-8 字符的边界，例如中文在 UTF-8 中占用三个字节，下面的代码就会崩溃：

```
 let s = "中国人";
 let a = &s[0..2];
 println!("{}",a);
```

> 因为我们只取 s 字符串的前两个字节，但是本例中每个汉字占用三个字节，因此没有落在边界处，也就是连 中 字都取不完整，此时程序会直接崩溃退出，如果改成 &s[0..3]，则可以正常通过编译。 因此，当你需要对字符串做切片索引操作时，需要格外小心这一点

字符串切片的类型标识是 &str，因此我们可以这样声明一个函数，输入 String 类型，返回它的切片：fn first_word(s: &String) -> &str 。

```
fn main() {
    let mut s = String::from("hello world");

    let word = first_word(&s);

    // s 被借用走了
    s.clear(); // error!

    println!("the first word is: {}", word);
}
fn first_word(s: &String) -> &str {
    &s[..1]
}
```

### 其它切片

因为切片是对集合的部分引用，因此不仅仅字符串有切片，其它集合类型也有，例如数组：

```
let a = [1, 2, 3, 4, 5];

let slice = &a[1..3];

assert_eq!(slice, &[2, 3]);
```

### 字符串字面量是切片

```
let s = "Hello, world!";
// 等价于
let s: &str = "Hello, world!";
```

## 字符串

- Rust 中的字符是 Unicode 类型，因此每个字符占据 4 个字节内存空间
- 字符串是由字符组成的连续集合，字符串是 UTF-8 编码，也就是字符串中的字符所占的字节数是变化的(1 - 4)，这样有助于大幅降低字符串所占用的内存空间。

Rust 在语言级别，只有一种字符串类型： str，它通常是以引用类型出现 &str，也就是上文提到的字符串切片。虽然语言级别只有上述的 str 类型，但是在标准库里，还有多种不同用途的字符串类型，其中使用最广的即是 String 类型。

- str 类型是硬编码进可执行文件，也无法被修改
- String 则是一个可增长、可改变且具有所有权的 UTF-8 编码字符串
- Rust 用户提到字符串时，往往指的就是 String 类型和 &str 字符串切片类型，这两个类型都是 UTF-8 编码

除了 String 类型的字符串，Rust 的标准库还提供了其他类型的字符串，例如 OsString， OsStr， CString 和 CStr 等，注意到这些名字都以 String 或者 Str 结尾了吗？它们分别对应的是具有所有权和被借用的变量。

### String 与 &str 的转换

- `String::from("hello,world")`
- `"hello,world".to_string()`

那么如何将 String 类型转为 &str 类型呢？答案很简单，取引用即可：
```
fn main() {
    let s = String::from("hello,world!");
    say_hello(&s);
    say_hello(&s[..]);
    say_hello(s.as_str());
}

fn say_hello(s: &str) {
    println!("{}",s);
}
```

### 字符串索引

在其它语言中，使用索引的方式访问字符串的某个字符或者子串是很正常的行为，但是在 Rust 中就会报错：

```
let s1 = String::from("hello");
let h = s1[0]; // 报错，String cannot be indexed by `{integer}`
```

### 深入字符串内部

字符串的底层的数据存储格式实际上是[ u8 ]，一个字节数组。对于 let hello = String::from("Hola"); 这行代码来说，Hola 的长度是 4 个字节，因为 "Hola" 中的每个字母在 UTF-8 编码中仅占用 1 个字节，但是对于下面的代码呢？

```
let hello = String::from("中国人");
```
如果问你该字符串多长，你可能会说 3，但是实际上是 9 个字节的长度，因为大部分常用汉字在 UTF-8 中的长度是 3 个字节，因此这种情况下对 hello 进行索引，访问 &hello[0] 没有任何意义，因为你取不到 中 这个字符，而是取到了这个字符三个字节中的第一个字节，这是一个非常奇怪而且难以理解的返回值。

还有一个原因导致了 Rust 不允许去索引字符串：因为索引操作，我们总是期望它的性能表现是 O(1)，然而对于 String 类型来说，无法保证这一点，因为 Rust 可能需要从 0 开始去遍历字符串来定位合法的字符。

### 字符串切片

字符串切片是非常危险的操作，因为切片的索引是通过字节来进行，但是字符串又是 UTF-8 编码，因此你无法保证索引的字节刚好落在字符的边界上，例如：

```
let hello = "中国人";

let s = &hello[0..2];
// 运行会崩溃
```

### 操作字符串

由于 String 是可变字符串，下面介绍 Rust 字符串的修改，添加，删除等常用方法：

#### 追加

```
fn main() {
    let mut s = String::from("Hello ");

    s.push_str("rust");
    println!("追加字符串 push_str() -> {}", s);

    s.push('!');
    println!("追加字符 push() -> {}", s);
}
```

#### 插入

```
fn main() {
    let mut s = String::from("Hello rust!");
    s.insert(5, ',');
    println!("插入字符 insert() -> {}", s);
    s.insert_str(6, " I like");
    println!("插入字符串 insert_str() -> {}", s);
}
```

#### 替换

1. replace
```
fn main() {
    let string_replace = String::from("I like rust. Learning rust is my favorite!");
    let new_string_replace = string_replace.replace("rust", "RUST");
    dbg!(new_string_replace);
}
```

2. replacen

方法可适用于 String 和 &str 类型。replacen() 方法接收三个参数，前两个参数与 replace() 方法一样，第三个参数则表示替换的个数。该方法是返回一个新的字符串，而不是操作原来的字符串。

```
fn main() {
    let string_replace = "I like rust. Learning rust is my favorite!";
    let new_string_replacen = string_replace.replacen("rust", "RUST", 1);
    dbg!(new_string_replacen);
}
```

3. replace_range

该方法仅适用于 String 类型。replace_range 接收两个参数，第一个参数是要替换字符串的范围（Range），第二个参数是新的字符串。该方法是直接操作原来的字符串，不会返回新的字符串。该方法需要使用 mut 关键字修饰。

```
fn main() {
    let mut string_replace_range = String::from("I like rust!");
    string_replace_range.replace_range(7..8, "R");
    dbg!(string_replace_range);
}
```

#### 删除

与字符串删除相关的方法有 4 个，它们分别是 pop()，remove()，truncate()，clear()。这四个方法仅适用于 String 类型。

1. pop

删除并返回字符串的最后一个字符

该方法是直接操作原来的字符串。但是存在返回值，其返回值是一个 Option 类型，如果字符串为空，则返回 None

```
fn main() {
    let mut string_pop = String::from("rust pop 中文!");
    let p1 = string_pop.pop();
    let p2 = string_pop.pop();
    dbg!(p1);
    dbg!(p2);
    dbg!(string_pop);
}
```
2. remove

删除并返回字符串中指定位置的字符

该方法是直接操作原来的字符串。但是存在返回值，其返回值是删除位置的字符串，只接收一个参数，表示该字符起始索引位置。remove() 方法是按照字节来处理字符串的，如果参数所给的位置不是合法的字符边界，则会发生错误。

```
fn main() {
    let mut string_remove = String::from("测试remove方法");
    println!(
        "string_remove 占 {} 个字节",
        std::mem::size_of_val(string_remove.as_str())
    );
    // 删除第一个汉字
    string_remove.remove(0);
    // 下面代码会发生错误
    // string_remove.remove(1);
    // 直接删除第二个汉字
    // string_remove.remove(3);
    dbg!(string_remove);
}
```

3. truncate

删除字符串中从指定位置开始到结尾的全部字符

该方法是直接操作原来的字符串。无返回值。该方法 truncate() 方法是按照字节来处理字符串的，如果参数所给的位置不是合法的字符边界，则会发生错误。

```
fn main() {
    let mut string_truncate = String::from("测试truncate");
    string_truncate.truncate(3);
    dbg!(string_truncate);
}
```

4. clear

清空字符串

该方法是直接操作原来的字符串。调用后，删除字符串中的所有字符，相当于 truncate() 方法参数为 0 的时候。

```
fn main() {
    let mut string_clear = String::from("string clear");
    string_clear.clear();
    dbg!(string_clear);
}
```

#### 连接

1. 使用 `+` 或者 `+=` 连接字符串

要求右边的参数必须为字符串的切片引用（Slice）类型。

其实当调用 `+` 的操作符时，相当于调用了 std::string 标准库中的 add() 方法，这里 add() 方法的第二个参数是一个引用的类型。

因此我们在使用 `+` 时， 必须传递切片引用类型。

不能直接传递 String 类型。

`+` 是返回一个新的字符串，所以变量声明可以不需要 mut 关键字修饰。

```
fn main() {
    let string_append = String::from("hello ");
    let string_rust = String::from("rust");
    // &string_rust会自动解引用为&str
    let result = string_append + &string_rust;
    let mut result = result + "!"; // `result + "!"` 中的 `result` 是不可变的
    result += "!!!";

    println!("连接字符串 + -> {}", result);
}
```

2. `add()`方法的定义

```
fn add(self, s: &str) -> String
```

例子

```
fn main() {
    let s1 = String::from("hello,");
    let s2 = String::from("world!");
    // 在下句中，s1的所有权被转移走了，因此后面不能再使用s1
    let s3 = s1 + &s2;
    assert_eq!(s3,"hello,world!");
    // 下面的语句如果去掉注释，就会报错
    // println!("{}",s1);
}
```

self 是 String 类型的字符串 s1，该函数说明，只能将 &str 类型的字符串切片添加到 String 类型的 s1 上，然后返回一个新的 String 类型，所以 let s3 = s1 + &s2; 就很好解释了，将 String 类型的 s1 与 &str 类型的 s2 进行相加，最终得到 String 类型的 s3。

由此可推，以下代码也是合法的：

```
let s1 = String::from("tic");
let s2 = String::from("tac");
let s3 = String::from("toe");

// String = String + &str + &str + &str + &str
let s = s1 + "-" + &s2 + "-" + &s3;
```

String + &str返回一个 String，然后再继续跟一个 &str 进行 + 操作，返回一个 String 类型，不断循环，最终生成一个 s，也是 String 类型

s1 这个变量通过调用 add() 方法后，所有权被转移到 add() 方法里面， add() 方法调用后就被释放了，同时 s1 也被释放了。再使用 s1 就会发生错误。

3. 使用 `format!` 连接字符串

`format!` 这种方式适用于 `String` 和 `&str` 。`format!` 的用法与 `print!` 的用法类似，示例代码如下：

```
fn main() {
    let s1 = "hello";
    let s2 = String::from("rust");
    let s = format!("{} {}!", s1, s2);
    println!("{}", s);
}
```

#### 字符串转义

```
fn main() {
    // 通过 \ + 字符的十六进制表示，转义输出一个字符
    let byte_escape = "I'm writing \x52\x75\x73\x74!";
    println!("What are you doing\x3F (\\x3F means ?) {}", byte_escape);

    // \u 可以输出一个 unicode 字符
    let unicode_codepoint = "\u{211D}";
    let character_name = "\"DOUBLE-STRUCK CAPITAL R\"";

    println!(
        "Unicode character {} (U+211D) is called {}",
        unicode_codepoint, character_name
    );

    // 换行了也会保持之前的字符串格式
    // 使用\忽略换行符
    let long_string = "String literals
                        can span multiple lines.
                        The linebreak and indentation here ->\
                        <- can be escaped too!";
    println!("{}", long_string);
}
```

当然，在某些情况下，可能你会希望保持字符串的原样，不要转义：

```
fn main() {
    println!("{}", "hello \\x52\\x75\\x73\\x74");
    let raw_str = r"Escapes don't work here: \x3F \u{211D}";
    println!("{}", raw_str);

    // 如果字符串包含双引号，可以在开头和结尾加 #
    let quotes = r#"And then I said: "There is no escape!""#;
    println!("{}", quotes);

    // 如果字符串中包含 # 号，
    // 可以在开头和结尾加多个 # 号，最多加255个，
    // 只需保证与字符串中连续 # 号的个数不超过开头和结尾的 # 号的个数即可
    let longer_delimiter = r###"A string with "# in it. And even "##!"###;
    println!("{}", longer_delimiter);
}
```

### 操作UTF-8字符串

#### 字符

如果你想要以 Unicode 字符的方式遍历字符串，最好的办法是使用 chars 方法，例如：

```
for c in "中国人".chars() {
    println!("{}", c);
}
```
这种方式是返回字符串的底层字节数组表现形式：

```
for b in "中国人".bytes() {
    println!("{}", b);
}
```

```
228
184
173
229
155
189
228
186
186
```

#### 获取子串

想要准确的从 UTF-8 字符串中获取子串是较为复杂的事情，例如想要从 holla中国人नमस्ते 这种变长的字符串中取出某一个子串，使用标准库你是做不到的。 你需要在 crates.io 上搜索 utf8 来寻找想要的功能。

可以考虑尝试下这个库：`utf8_slice`。

## 字符串深度剖析

那么问题来了，为啥 String 可变，而字符串字面值 str 却不可以？

就字符串字面值来说，我们在编译时就知道其内容，最终字面值文本被直接硬编码进可执行文件中，这使得字符串字面值快速且高效，这主要得益于字符串字面值的不可变性。

对于 String 类型，为了支持一个可变、可增长的文本片段，需要在堆上分配一块在编译时未知大小的内存来存放内容，这些都是在程序运行时完成的：
- 首先向操作系统请求内存来存放 String 对象
- 在使用完成后，将内存释放，归还给操作系统

其中第一部分由 String::from 完成，它创建了一个全新的 String。

到了第二部分，就是百家齐放的环节，在有垃圾回收 GC 的语言中，GC 来负责标记并清除这些不再使用的内存对象，这个过程都是自动完成，无需开发者关心，非常简单好用；但是在无 GC 的语言中，需要开发者手动去释放这些内存对象，就像创建对象需要通过编写代码来完成一样，未能正确释放对象造成的后果简直不可估量。

对于 Rust 而言，安全和性能是写到骨子里的核心特性，Rust 的开发者想出了一个无比惊艳的办法：变量在离开作用域后，就自动释放其占用的内存。

与其它系统编程语言的 free 函数相同，Rust 也提供了一个释放内存的函数： drop，但是不同的是，其它语言要手动调用 free 来释放每一个变量占用的内存，而 Rust 则在变量离开作用域时，自动调用 drop 函数：上面代码中，Rust 在结尾的 } 处自动调用 drop。

## 元组

元组是由多种类型组合到一起形成的，因此它是复合类型，元组的长度是固定的，元组中元素的顺序也是固定的。

可以通过以下语法创建一个元组：

```
fn main() {
    let tup: (i32, f64, u8) = (500, 6.4, 1);
}
```

可以使用模式匹配或者 . 操作符来获取元组中的值。

### 用模式匹配解构元组

```
fn main() {
    let tup = (500, 6.4, 1);

    let (x, y, z) = tup;

    println!("The value of y is: {}", y);
}
```

上述代码首先创建一个元组，然后将其绑定到 tup 上，接着使用 let (x, y, z) = tup; 来完成一次模式匹配，因为元组是 (n1, n2, n3) 形式的，因此我们用一模一样的 (x, y, z) 形式来进行匹配，元组中对应的值会绑定到变量 x， y， z上。这就是解构：用同样的形式把一个复杂对象中的值匹配出来。

### 用`.`来访问元组

模式匹配可以让我们一次性把元组中的值全部或者部分获取出来，如果只想要访问某个特定元素，那模式匹配就略显繁琐，对此，Rust 提供了 . 的访问方式：

```
fn main() {
    let x: (i32, f64, u8) = (500, 6.4, 1);

    let five_hundred = x.0;

    let six_point_four = x.1;

    let one = x.2;
}
```
