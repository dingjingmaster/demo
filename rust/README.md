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
