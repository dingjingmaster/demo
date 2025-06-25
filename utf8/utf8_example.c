#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "utf8_validator.h"

/**
 * 打印字节序列（十六进制格式）
 */
void print_bytes(const uint8_t *data, size_t len) {
    printf("字节序列: ");
    for (size_t i = 0; i < len; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
}

/**
 * 打印 UTF-8 验证结果
 */
void print_utf8_result(utf8_result_t result) {
    switch (result) {
        case UTF8_VALID:
            printf("结果: ✓ 有效的完整 UTF-8 字符\n");
            break;
        case UTF8_INCOMPLETE:
            printf("结果: ⚠ 不完整的 UTF-8 字符（需要更多字节）\n");
            break;
        case UTF8_INVALID:
            printf("结果: ✗ 无效的 UTF-8 序列\n");
            break;
    }
}

/**
 * 测试单个 UTF-8 字符
 */
void test_utf8_char(const char *description, const uint8_t *data, size_t len) {
    printf("\n=== %s ===\n", description);
    print_bytes(data, len);
    
    size_t consumed = 0;
    utf8_result_t result = validate_utf8_char(data, len, &consumed);
    print_utf8_result(result);
    
    if (result == UTF8_VALID) {
        printf("消耗字节数: %zu\n", consumed);
        printf("期望长度: %d\n", get_utf8_char_length(data[0]));
    }
}

/**
 * 测试 UTF-8 字符串
 */
void test_utf8_string(const char *description, const char *str) {
    printf("\n=== %s ===\n", description);
    printf("字符串: \"%s\"\n", str);
    
    const uint8_t *data = (const uint8_t *)str;
    size_t len = strlen(str);
    
    print_bytes(data, len);
    
    bool is_valid = is_valid_utf8_string(data, len);
    printf("整体验证: %s\n", is_valid ? "✓ 有效" : "✗ 无效");
    
    if (is_valid) {
        int char_count = count_utf8_chars(data, len);
        printf("字符数: %d\n", char_count);
        printf("字节数: %zu\n", len);
    }
}

int main() {
    // 设置本地化以正确显示中文
    setlocale(LC_ALL, "");
    
    printf("UTF-8 字符验证器演示程序\n");
    printf("=======================\n");

    // 测试 1: ASCII 字符
    uint8_t ascii[] = {0x41}; // 'A'
    test_utf8_char("ASCII 字符 'A'", ascii, sizeof(ascii));

    // 测试 2: 2字节 UTF-8 字符（拉丁扩展）
    uint8_t latin[] = {0xC3, 0xA9}; // 'é'
    test_utf8_char("2字节 UTF-8 字符 'é'", latin, sizeof(latin));

    // 测试 3: 3字节 UTF-8 字符（中文）
    uint8_t chinese[] = {0xE4, 0xB8, 0xAD}; // '中'
    test_utf8_char("3字节 UTF-8 字符 '中'", chinese, sizeof(chinese));

    // 测试 4: 4字节 UTF-8 字符（表情符号）
    uint8_t emoji[] = {0xF0, 0x9F, 0x98, 0x80}; // '😀'
    test_utf8_char("4字节 UTF-8 字符 '😀'", emoji, sizeof(emoji));

    // 测试 5: 不完整的序列
    uint8_t incomplete[] = {0xE4, 0xB8}; // 缺少最后一个字节
    test_utf8_char("不完整的 UTF-8 序列", incomplete, sizeof(incomplete));

    // 测试 6: 无效的序列
    uint8_t invalid[] = {0xFF, 0xFE}; // 无效的开始字节
    test_utf8_char("无效的 UTF-8 序列", invalid, sizeof(invalid));

    // 测试 7: 过长编码（无效）
    uint8_t overlong[] = {0xC0, 0x80}; // 过长编码的 NULL
    test_utf8_char("过长编码（无效）", overlong, sizeof(overlong));

    // 测试字符串
    test_utf8_string("纯 ASCII", "Hello World");
    test_utf8_string("混合字符串", "Hello 世界 🌍");
    test_utf8_string("纯中文", "你好世界");
    test_utf8_string("包含表情符号", "编程很有趣 😊🚀");

    // 测试无效字符串
    printf("\n=== 无效字符串测试 ===\n");
    uint8_t invalid_string[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0xFF, 0x20, 0xE4, 0xB8, 0xAD};
    printf("包含无效字节的字符串:\n");
    print_bytes(invalid_string, sizeof(invalid_string));
    bool is_valid = is_valid_utf8_string(invalid_string, sizeof(invalid_string));
    printf("整体验证: %s\n", is_valid ? "✓ 有效" : "✗ 无效");

    printf("\n=== 逐字符分析演示 ===\n");
    const char *mixed_str = "Hi中🌍";
    const uint8_t *data = (const uint8_t *)mixed_str;
    size_t len = strlen(mixed_str);
    
    printf("分析字符串: \"%s\"\n", mixed_str);
    print_bytes(data, len);
    
    size_t i = 0;
    int char_num = 1;
    while (i < len) {
        size_t consumed = 0;
        utf8_result_t result = validate_utf8_char(data + i, len - i, &consumed);
        
        printf("\n字符 %d (位置 %zu):\n", char_num++, i);
        print_bytes(data + i, consumed > 0 ? consumed : 1);
        print_utf8_result(result);
        
        if (result == UTF8_VALID) {
            printf("长度: %zu 字节\n", consumed);
            i += consumed;
        } else {
            i++; // 跳过无效字节
        }
    }

    return 0;
} 