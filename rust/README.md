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
