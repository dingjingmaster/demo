# UTF-8 字符验证器

一个用 Linux C 实现的完整 UTF-8 字符验证库，可以判断字节序列是否构成完整有效的 UTF-8 字符。

## 功能特性

- ✅ **完整性检查**: 判断 UTF-8 字符序列是否完整
- ✅ **有效性验证**: 检查 UTF-8 编码是否符合标准
- ✅ **过长编码检测**: 识别并拒绝过长编码
- ✅ **代理对检测**: 检测无效的代理对范围
- ✅ **字符计数**: 计算 UTF-8 字符串中的字符数（非字节数）
- ✅ **字符串验证**: 验证整个字符串是否为有效 UTF-8

## UTF-8 编码规则

UTF-8 是一种变长编码，每个字符可以用 1-4 个字节表示：

| 字符范围 | 字节数 | 第1字节 | 第2字节 | 第3字节 | 第4字节 |
|----------|--------|---------|---------|---------|---------|
| U+0000 - U+007F | 1 | 0xxxxxxx | - | - | - |
| U+0080 - U+07FF | 2 | 110xxxxx | 10xxxxxx | - | - |
| U+0800 - U+FFFF | 3 | 1110xxxx | 10xxxxxx | 10xxxxxx | - |
| U+10000 - U+10FFFF | 4 | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |

## 文件结构

```
├── utf8_validator.h    # 头文件，包含函数声明
├── utf8_validator.c    # 核心实现，UTF-8 验证逻辑
├── utf8_example.c      # 使用示例和测试程序
├── Makefile           # 编译脚本
└── README.md          # 本文件
```

## 快速开始

### 编译

```bash
# 编译所有目标
make all

# 或者只编译示例程序
make utf8_example

# 编译静态库
make libutf8validator.a
```

### 运行示例

```bash
make run
```

### 基本使用

```c
#include "utf8_validator.h"

// 检查单个 UTF-8 字符
const uint8_t data[] = {0xE4, 0xB8, 0xAD}; // "中"
size_t consumed = 0;
utf8_result_t result = validate_utf8_char(data, sizeof(data), &consumed);

if (result == UTF8_VALID) {
    printf("有效的 UTF-8 字符，长度: %zu 字节\n", consumed);
}

// 验证整个字符串
const char *str = "Hello 世界 🌍";
if (is_valid_utf8_string((const uint8_t*)str, strlen(str))) {
    printf("有效的 UTF-8 字符串\n");
    int char_count = count_utf8_chars((const uint8_t*)str, strlen(str));
    printf("包含 %d 个字符\n", char_count);
}
```

## API 参考

### 核心函数

#### `validate_utf8_char()`
```c
utf8_result_t validate_utf8_char(const uint8_t *data, size_t len, size_t *consumed);
```
验证单个 UTF-8 字符是否完整且有效。

**参数:**
- `data`: 指向字节序列的指针
- `len`: 可用的字节数
- `consumed`: 输出参数，实际消耗的字节数

**返回值:**
- `UTF8_VALID`: 完整有效的 UTF-8 字符
- `UTF8_INCOMPLETE`: 不完整（需要更多字节）
- `UTF8_INVALID`: 无效的 UTF-8 序列

#### `is_valid_utf8_string()`
```c
bool is_valid_utf8_string(const uint8_t *data, size_t len);
```
验证整个字符串是否为有效的 UTF-8。

#### `count_utf8_chars()`
```c
int count_utf8_chars(const uint8_t *data, size_t len);
```
计算 UTF-8 字符串中的字符数（非字节数）。

### 辅助函数

#### `get_utf8_char_length()`
```c
int get_utf8_char_length(uint8_t first_byte);
```
根据第一个字节获取 UTF-8 字符的期望长度。

#### `is_utf8_start_byte()`
```c
bool is_utf8_start_byte(uint8_t byte);
```
判断字节是否是 UTF-8 序列的开始字节。

#### `is_utf8_continuation_byte()`
```c
bool is_utf8_continuation_byte(uint8_t byte);
```
判断字节是否是 UTF-8 序列的继续字节。

## 验证规则

本库严格按照 UTF-8 标准进行验证，包括：

1. **字节格式检查**: 确保每个字节符合 UTF-8 格式
2. **长度一致性**: 验证实际长度与第一字节指示的长度一致
3. **最短编码**: 拒绝过长编码（如用 2 字节编码 ASCII 字符）
4. **代码点范围**: 检查代码点是否在有效范围内
5. **代理对检测**: 拒绝 UTF-16 代理对范围（U+D800-U+DFFF）
6. **最大值检查**: 确保 4 字节序列不超过 U+10FFFF

## 测试用例

示例程序包含以下测试：

- ASCII 字符 (1 字节)
- 拉丁扩展字符 (2 字节)
- 中日韩字符 (3 字节)  
- 表情符号 (4 字节)
- 不完整序列
- 无效序列
- 过长编码
- 混合字符串

## 内存管理

- 所有函数都是纯函数，不分配内存
- 参数验证确保不会访问无效内存
- 使用 `valgrind` 验证无内存泄漏

## 编译选项

```bash
# 调试版本
make debug

# 内存检查
make memcheck

# 系统安装
make install

# 清理
make clean
```

## 系统要求

- Linux 系统
- GCC 编译器
- 支持 C99 标准
- 可选：valgrind（用于内存检查）

## 许可证

本项目采用 MIT 许可证。

## 参考资料

- [RFC 3629 - UTF-8](https://tools.ietf.org/html/rfc3629)
- [Unicode Standard](https://www.unicode.org/standard/standard.html)
- [UTF-8 Wikipedia](https://en.wikipedia.org/wiki/UTF-8) 