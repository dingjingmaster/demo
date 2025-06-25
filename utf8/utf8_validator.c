#include "utf8_validator.h"

/**
 * 判断单个字节是否是 UTF-8 序列的开始字节
 */
bool is_utf8_start_byte(uint8_t byte) {
    // ASCII (0xxxxxxx) 或 多字节序列开始 (110xxxxx, 1110xxxx, 11110xxx)
    return (byte & 0x80) == 0x00 ||  // ASCII
           (byte & 0xE0) == 0xC0 ||  // 2字节序列开始
           (byte & 0xF0) == 0xE0 ||  // 3字节序列开始
           (byte & 0xF8) == 0xF0;    // 4字节序列开始
}

/**
 * 判断单个字节是否是 UTF-8 序列的继续字节
 */
bool is_utf8_continuation_byte(uint8_t byte) {
    // 继续字节格式：10xxxxxx
    return (byte & 0xC0) == 0x80;
}

/**
 * 获取 UTF-8 字符的期望长度（基于第一个字节）
 */
int get_utf8_char_length(uint8_t first_byte) {
    if ((first_byte & 0x80) == 0x00) {
        // ASCII: 0xxxxxxx
        return 1;
    } else if ((first_byte & 0xE0) == 0xC0) {
        // 2字节: 110xxxxx
        return 2;
    } else if ((first_byte & 0xF0) == 0xE0) {
        // 3字节: 1110xxxx
        return 3;
    } else if ((first_byte & 0xF8) == 0xF0) {
        // 4字节: 11110xxx
        return 4;
    } else {
        // 无效的开始字节
        return 0;
    }
}

/**
 * 验证 UTF-8 字符序列是否完整且有效
 */
utf8_result_t validate_utf8_char(const uint8_t *data, size_t len, size_t *consumed) {
    if (!data || len == 0) {
        if (consumed) *consumed = 0;
        return UTF8_INCOMPLETE;
    }

    // 获取期望的字符长度
    int expected_len = get_utf8_char_length(data[0]);
    if (expected_len == 0) {
        if (consumed) *consumed = 0;
        return UTF8_INVALID;
    }

    // 检查是否有足够的字节
    if (len < expected_len) {
        if (consumed) *consumed = 0;
        return UTF8_INCOMPLETE;
    }

    // 验证继续字节
    for (int i = 1; i < expected_len; i++) {
        if (!is_utf8_continuation_byte(data[i])) {
            if (consumed) *consumed = 0;
            return UTF8_INVALID;
        }
    }

    // 检查编码的有效性（避免过长编码）
    uint32_t codepoint = 0;
    switch (expected_len) {
        case 1:
            codepoint = data[0];
            break;
        case 2:
            codepoint = ((data[0] & 0x1F) << 6) | (data[1] & 0x3F);
            // 最小值检查：2字节序列最小应该是 0x80
            if (codepoint < 0x80) {
                if (consumed) *consumed = 0;
                return UTF8_INVALID;
            }
            break;
        case 3:
            codepoint = ((data[0] & 0x0F) << 12) | 
                       ((data[1] & 0x3F) << 6) | 
                       (data[2] & 0x3F);
            // 最小值检查：3字节序列最小应该是 0x800
            // 同时检查代理对范围 (0xD800-0xDFFF)
            if (codepoint < 0x800 || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
                if (consumed) *consumed = 0;
                return UTF8_INVALID;
            }
            break;
        case 4:
            codepoint = ((data[0] & 0x07) << 18) | 
                       ((data[1] & 0x3F) << 12) | 
                       ((data[2] & 0x3F) << 6) | 
                       (data[3] & 0x3F);
            // 最小值检查：4字节序列最小应该是 0x10000
            // 最大值检查：Unicode 最大值是 0x10FFFF
            if (codepoint < 0x10000 || codepoint > 0x10FFFF) {
                if (consumed) *consumed = 0;
                return UTF8_INVALID;
            }
            break;
    }

    if (consumed) *consumed = expected_len;
    return UTF8_VALID;
}

/**
 * 验证整个字符串是否为有效的 UTF-8
 */
bool is_valid_utf8_string(const uint8_t *data, size_t len) {
    if (!data) return len == 0;

    size_t i = 0;
    while (i < len) {
        size_t consumed = 0;
        utf8_result_t result = validate_utf8_char(data + i, len - i, &consumed);
        
        if (result != UTF8_VALID) {
            return false;
        }
        
        i += consumed;
    }
    
    return true;
}

/**
 * 计算 UTF-8 字符串中的字符数（非字节数）
 */
int count_utf8_chars(const uint8_t *data, size_t len) {
    if (!data) return len == 0 ? 0 : -1;

    int char_count = 0;
    size_t i = 0;
    
    while (i < len) {
        size_t consumed = 0;
        utf8_result_t result = validate_utf8_char(data + i, len - i, &consumed);
        
        if (result != UTF8_VALID) {
            return -1;  // 无效的 UTF-8 序列
        }
        
        char_count++;
        i += consumed;
    }
    
    return char_count;
} 