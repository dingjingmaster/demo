#include "filter.h"
#include <stdio.h>
#include <string.h>

/**
 * 获取 UTF-8 字符的 Unicode 代码点
 */
uint32_t utf8_to_codepoint(const uint8_t *data, size_t len) {
    if (!data || len == 0) return 0;
    
    int char_len = get_utf8_char_length(data[0]);
    if (char_len == 0 || len < char_len) return 0;
    
    uint32_t codepoint = 0;
    switch (char_len) {
        case 1:
            codepoint = data[0];
            break;
        case 2:
            codepoint = ((data[0] & 0x1F) << 6) | (data[1] & 0x3F);
            break;
        case 3:
            codepoint = ((data[0] & 0x0F) << 12) | 
                       ((data[1] & 0x3F) << 6) | 
                       (data[2] & 0x3F);
            break;
        case 4:
            codepoint = ((data[0] & 0x07) << 18) | 
                       ((data[1] & 0x3F) << 12) | 
                       ((data[2] & 0x3F) << 6) | 
                       (data[3] & 0x3F);
            break;
    }
    
    return codepoint;
}

/**
 * 判断 Unicode 代码点是否为可见字符
 */
bool is_visible_codepoint(uint32_t codepoint) {
    // ASCII 可见字符范围
    if (codepoint >= 0x20 && codepoint <= 0x7E) {
        return true;
    }
    
    // 2字节范围的可见字符
    if (is_visible_2byte_char(codepoint)) {
        return true;
    }
    
    // 3字节范围的可见字符
    if (is_visible_3byte_char(codepoint)) {
        return true;
    }
    
    // 4字节范围的可见字符
    if (is_visible_4byte_char(codepoint)) {
        return true;
    }
    
    return false;
}

/**
 * 判断 2字节 UTF-8 字符是否为可见字符
 */
bool is_visible_2byte_char(uint32_t codepoint) {
    // 2字节范围: U+0080 - U+07FF
    if (codepoint < 0x80 || codepoint > 0x7FF) {
        return false;
    }
    
    // 拉丁扩展补充 (Latin-1 Supplement): U+00A0 - U+00FF
    if (codepoint >= 0x00A0 && codepoint <= 0x00FF) {
        return true;
    }
    
    // 拉丁扩展-A (Latin Extended-A): U+0100 - U+017F
    if (codepoint >= 0x0100 && codepoint <= 0x017F) {
        return true;
    }
    
    // 拉丁扩展-B (Latin Extended-B): U+0180 - U+024F
    if (codepoint >= 0x0180 && codepoint <= 0x024F) {
        return true;
    }
    
    // IPA 扩展 (IPA Extensions): U+0250 - U+02AF
    if (codepoint >= 0x0250 && codepoint <= 0x02AF) {
        return true;
    }
    
    // 希腊文和科普特文 (Greek and Coptic): U+0370 - U+03FF
    if (codepoint >= 0x0370 && codepoint <= 0x03FF) {
        return true;
    }
    
    // 西里尔文 (Cyrillic): U+0400 - U+04FF
    if (codepoint >= 0x0400 && codepoint <= 0x04FF) {
        return true;
    }
    
    // 西里尔文补充 (Cyrillic Supplement): U+0500 - U+052F
    if (codepoint >= 0x0500 && codepoint <= 0x052F) {
        return true;
    }
    
    // 亚美尼亚文 (Armenian): U+0530 - U+058F
    if (codepoint >= 0x0530 && codepoint <= 0x058F) {
        return true;
    }
    
    // 希伯来文 (Hebrew): U+0590 - U+05FF
    if (codepoint >= 0x0590 && codepoint <= 0x05FF) {
        return true;
    }
    
    // 阿拉伯文 (Arabic): U+0600 - U+06FF
    if (codepoint >= 0x0600 && codepoint <= 0x06FF) {
        return true;
    }
    
    return false;
}

/**
 * 判断 3字节 UTF-8 字符是否为可见字符
 */
bool is_visible_3byte_char(uint32_t codepoint) {
    // 3字节范围: U+0800 - U+FFFF
    if (codepoint < 0x0800 || codepoint > 0xFFFF) {
        return false;
    }
    
    // 排除代理对范围
    if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
        return false;
    }
    
    // 一般标点符号 (General Punctuation): U+2000 - U+206F
    if (codepoint >= 0x2000 && codepoint <= 0x206F) {
        return true;
    }
    
    // 上标和下标 (Superscripts and Subscripts): U+2070 - U+209F
    if (codepoint >= 0x2070 && codepoint <= 0x209F) {
        return true;
    }
    
    // 货币符号 (Currency Symbols): U+20A0 - U+20CF
    if (codepoint >= 0x20A0 && codepoint <= 0x20CF) {
        return true;
    }
    
    // 数学符号 (Mathematical Operators): U+2200 - U+22FF
    if (codepoint >= 0x2200 && codepoint <= 0x22FF) {
        return true;
    }
    
    // 杂项技术符号 (Miscellaneous Technical): U+2300 - U+23FF
    if (codepoint >= 0x2300 && codepoint <= 0x23FF) {
        return true;
    }
    
    // 控制图片 (Control Pictures): U+2400 - U+243F
    if (codepoint >= 0x2400 && codepoint <= 0x243F) {
        return true;
    }
    
    // 几何形状 (Geometric Shapes): U+25A0 - U+25FF
    if (codepoint >= 0x25A0 && codepoint <= 0x25FF) {
        return true;
    }
    
    // 杂项符号 (Miscellaneous Symbols): U+2600 - U+26FF
    if (codepoint >= 0x2600 && codepoint <= 0x26FF) {
        return true;
    }
    
    // 中日韩符号和标点 (CJK Symbols and Punctuation): U+3000 - U+303F
    if (codepoint >= 0x3000 && codepoint <= 0x303F) {
        return true;
    }
    
    // 日文平假名 (Hiragana): U+3040 - U+309F
    if (codepoint >= 0x3040 && codepoint <= 0x309F) {
        return true;
    }
    
    // 日文片假名 (Katakana): U+30A0 - U+30FF
    if (codepoint >= 0x30A0 && codepoint <= 0x30FF) {
        return true;
    }
    
    // 韩文注音字母 (Bopomofo): U+3100 - U+312F
    if (codepoint >= 0x3100 && codepoint <= 0x312F) {
        return true;
    }
    
    // 韩文兼容字母 (Hangul Compatibility Jamo): U+3130 - U+318F
    if (codepoint >= 0x3130 && codepoint <= 0x318F) {
        return true;
    }
    
    // 中日韩统一表意文字 (CJK Unified Ideographs): U+4E00 - U+9FFF
    if (codepoint >= 0x4E00 && codepoint <= 0x9FFF) {
        return true;
    }
    
    // 韩文音节 (Hangul Syllables): U+AC00 - U+D7AF
    if (codepoint >= 0xAC00 && codepoint <= 0xD7AF) {
        return true;
    }
    
    // 中日韩兼容表意文字 (CJK Compatibility Ideographs): U+F900 - U+FAFF
    if (codepoint >= 0xF900 && codepoint <= 0xFAFF) {
        return true;
    }
    
    // 字母表示形式 (Alphabetic Presentation Forms): U+FB00 - U+FB4F
    if (codepoint >= 0xFB00 && codepoint <= 0xFB4F) {
        return true;
    }
    
    // 阿拉伯文表示形式-A (Arabic Presentation Forms-A): U+FB50 - U+FDFF
    if (codepoint >= 0xFB50 && codepoint <= 0xFDFF) {
        return true;
    }
    
    // 组合半标记 (Combining Half Marks): U+FE20 - U+FE2F
    if (codepoint >= 0xFE20 && codepoint <= 0xFE2F) {
        return true;
    }
    
    // 中日韩兼容形式 (CJK Compatibility Forms): U+FE30 - U+FE4F
    if (codepoint >= 0xFE30 && codepoint <= 0xFE4F) {
        return true;
    }
    
    // 小形式变体 (Small Form Variants): U+FE50 - U+FE6F
    if (codepoint >= 0xFE50 && codepoint <= 0xFE6F) {
        return true;
    }
    
    // 阿拉伯文表示形式-B (Arabic Presentation Forms-B): U+FE70 - U+FEFF
    if (codepoint >= 0xFE70 && codepoint <= 0xFEFF) {
        return true;
    }
    
    // 半角和全角形式 (Halfwidth and Fullwidth Forms): U+FF00 - U+FFEF
    if (codepoint >= 0xFF00 && codepoint <= 0xFFEF) {
        return true;
    }
    
    return false;
}

/**
 * 判断 4字节 UTF-8 字符是否为可见字符
 */
bool is_visible_4byte_char(uint32_t codepoint) {
    // 4字节范围: U+10000 - U+10FFFF
    if (codepoint < 0x10000 || codepoint > 0x10FFFF) {
        return false;
    }
    
    // 表情符号 (Emoticons): U+1F600 - U+1F64F
    if (codepoint >= 0x1F600 && codepoint <= 0x1F64F) {
        return true;
    }
    
    // 杂项符号和象形文字 (Miscellaneous Symbols and Pictographs): U+1F300 - U+1F5FF
    if (codepoint >= 0x1F300 && codepoint <= 0x1F5FF) {
        return true;
    }
    
    // 传输和地图符号 (Transport and Map Symbols): U+1F680 - U+1F6FF
    if (codepoint >= 0x1F680 && codepoint <= 0x1F6FF) {
        return true;
    }
    
    // 补充符号和象形文字 (Supplemental Symbols and Pictographs): U+1F900 - U+1F9FF
    if (codepoint >= 0x1F900 && codepoint <= 0x1F9FF) {
        return true;
    }
    
    // 中日韩统一表意文字扩展B (CJK Unified Ideographs Extension B): U+20000 - U+2A6DF
    if (codepoint >= 0x20000 && codepoint <= 0x2A6DF) {
        return true;
    }
    
    // 中日韩统一表意文字扩展C (CJK Unified Ideographs Extension C): U+2A700 - U+2B73F
    if (codepoint >= 0x2A700 && codepoint <= 0x2B73F) {
        return true;
    }
    
    // 中日韩统一表意文字扩展D (CJK Unified Ideographs Extension D): U+2B740 - U+2B81F
    if (codepoint >= 0x2B740 && codepoint <= 0x2B81F) {
        return true;
    }
    
    // 中日韩兼容表意文字补充 (CJK Compatibility Ideographs Supplement): U+2F800 - U+2FA1F
    if (codepoint >= 0x2F800 && codepoint <= 0x2FA1F) {
        return true;
    }
    
    return false;
}

/**
 * 获取 UTF-8 字符的类型
 */
utf8_char_type_t get_utf8_char_type(const uint8_t *data, size_t len) {
    if (!data || len == 0) {
        return UTF8_CHAR_INVALID;
    }
    
    int char_len = get_utf8_char_length(data[0]);
    if (char_len == 0 || len < char_len) {
        return UTF8_CHAR_INVALID;
    }
    
    // 验证字符是否有效
    size_t consumed = 0;
    utf8_result_t result = validate_utf8_char(data, len, &consumed);
    if (result != UTF8_VALID) {
        return UTF8_CHAR_INVALID;
    }
    
    switch (char_len) {
        case 1: return UTF8_CHAR_1BYTE;
        case 2: return UTF8_CHAR_2BYTE;
        case 3: return UTF8_CHAR_3BYTE;
        case 4: return UTF8_CHAR_4BYTE;
        default: return UTF8_CHAR_INVALID;
    }
}

/**
 * 过滤 UTF-8 字符串，保留指定类型的可见字符
 */
int filter_utf8_string(const uint8_t *input, size_t input_len,
                      uint8_t *output, size_t output_size,
                      const utf8_filter_options_t *options) {
    if (!input || !output || !options) {
        return -1;
    }
    
    size_t input_pos = 0;
    size_t output_pos = 0;
    
    while (input_pos < input_len && output_pos < output_size) {
        size_t consumed = 0;
        utf8_result_t result = validate_utf8_char(input + input_pos, 
                                                input_len - input_pos, &consumed);
        
        if (result != UTF8_VALID) {
            input_pos++;
            continue;
        }
        
        uint32_t codepoint = utf8_to_codepoint(input + input_pos, consumed);
        utf8_char_type_t char_type = get_utf8_char_type(input + input_pos, consumed);
        
        bool should_include = false;
        
        // 检查是否应该包含这个字符
        switch (char_type) {
            case UTF8_CHAR_1BYTE:
                if (options->include_1byte && is_visible_codepoint(codepoint)) {
                    should_include = true;
                }
                break;
            case UTF8_CHAR_2BYTE:
                if (options->include_2byte && is_visible_2byte_char(codepoint)) {
                    should_include = true;
                }
                break;
            case UTF8_CHAR_3BYTE:
                if (options->include_3byte && is_visible_3byte_char(codepoint)) {
                    should_include = true;
                }
                break;
            case UTF8_CHAR_4BYTE:
                if (options->include_4byte && is_visible_4byte_char(codepoint)) {
                    should_include = true;
                }
                break;
            default:
                break;
        }
        
        if (should_include) {
            // 检查输出缓冲区是否有足够空间
            if (output_pos + consumed > output_size) {
                break;
            }
            
            // 复制字符到输出缓冲区
            memcpy(output + output_pos, input + input_pos, consumed);
            output_pos += consumed;
        }
        
        input_pos += consumed;
    }
    
    return (int)output_pos;
}

/**
 * 统计不同字节长度的可见字符数量
 */
int count_visible_chars_by_type(const uint8_t *input, size_t input_len,
                               int *count_1byte, int *count_2byte,
                               int *count_3byte, int *count_4byte) {
    if (!input) return -1;
    
    if (count_1byte) *count_1byte = 0;
    if (count_2byte) *count_2byte = 0;
    if (count_3byte) *count_3byte = 0;
    if (count_4byte) *count_4byte = 0;
    
    size_t pos = 0;
    int total_visible = 0;
    
    while (pos < input_len) {
        size_t consumed = 0;
        utf8_result_t result = validate_utf8_char(input + pos, input_len - pos, &consumed);
        
        if (result != UTF8_VALID) {
            pos++;
            continue;
        }
        
        uint32_t codepoint = utf8_to_codepoint(input + pos, consumed);
        utf8_char_type_t char_type = get_utf8_char_type(input + pos, consumed);
        
        switch (char_type) {
            case UTF8_CHAR_1BYTE:
                if (is_visible_codepoint(codepoint)) {
                    if (count_1byte) (*count_1byte)++;
                    total_visible++;
                }
                break;
            case UTF8_CHAR_2BYTE:
                if (is_visible_2byte_char(codepoint)) {
                    if (count_2byte) (*count_2byte)++;
                    total_visible++;
                }
                break;
            case UTF8_CHAR_3BYTE:
                if (is_visible_3byte_char(codepoint)) {
                    if (count_3byte) (*count_3byte)++;
                    total_visible++;
                }
                break;
            case UTF8_CHAR_4BYTE:
                if (is_visible_4byte_char(codepoint)) {
                    if (count_4byte) (*count_4byte)++;
                    total_visible++;
                }
                break;
            default:
                break;
        }
        
        pos += consumed;
    }
    
    return total_visible;
}

/**
 * 打印字符的详细信息
 */
void print_char_info(const uint8_t *data, size_t len) {
    if (!data || len == 0) {
        printf("无效字符\n");
        return;
    }
    
    uint32_t codepoint = utf8_to_codepoint(data, len);
    utf8_char_type_t char_type = get_utf8_char_type(data, len);
    
    printf("字节序列: ");
    for (size_t i = 0; i < len; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
    
    printf("Unicode: U+%04X\n", codepoint);
    
    const char *type_names[] = {
        "1字节 (ASCII)",
        "2字节",
        "3字节", 
        "4字节",
        "无效"
    };
    printf("类型: %s\n", type_names[char_type]);
    
    printf("可见: %s\n", is_visible_codepoint(codepoint) ? "是" : "否");
    
    if (len <= 4) {
        printf("字符: ");
        for (size_t i = 0; i < len; i++) {
            printf("%c", data[i] >= 32 && data[i] <= 126 ? data[i] : '?');
        }
        printf("\n");
    }
} 
