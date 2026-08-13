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





