#ifndef UTF8_FILTER_H
#define UTF8_FILTER_H

#include "validator.h"

/**
 * UTF-8 字符类别枚举
 */
typedef enum {
    UTF8_CHAR_1BYTE,     // 1字节字符 (ASCII)
    UTF8_CHAR_2BYTE,     // 2字节字符
    UTF8_CHAR_3BYTE,     // 3字节字符
    UTF8_CHAR_4BYTE,     // 4字节字符
    UTF8_CHAR_INVALID    // 无效字符
} utf8_char_type_t;

/**
 * 可见字符过滤选项
 */
typedef struct {
    bool include_1byte;     // 包含1字节可见字符
    bool include_2byte;     // 包含2字节可见字符
    bool include_3byte;     // 包含3字节可见字符
    bool include_4byte;     // 包含4字节可见字符
    bool exclude_control;   // 排除控制字符
    bool exclude_format;    // 排除格式字符
    bool exclude_private;   // 排除私用区字符
} utf8_filter_options_t;

/**
 * 获取 UTF-8 字符的 Unicode 代码点
 * @param data UTF-8 字节序列
 * @param len 字节序列长度
 * @return Unicode 代码点，失败返回 0
 */
uint32_t utf8_to_codepoint(const uint8_t *data, size_t len);

/**
 * 判断 Unicode 代码点是否为可见字符
 * @param codepoint Unicode 代码点
 * @return true 如果是可见字符，false 否则
 */
bool is_visible_codepoint(uint32_t codepoint);

/**
 * 判断 2字节 UTF-8 字符是否为可见字符
 * @param codepoint Unicode 代码点
 * @return true 如果是 2字节可见字符，false 否则
 */
bool is_visible_2byte_char(uint32_t codepoint);

/**
 * 判断 3字节 UTF-8 字符是否为可见字符
 * @param codepoint Unicode 代码点
 * @return true 如果是 3字节可见字符，false 否则
 */
bool is_visible_3byte_char(uint32_t codepoint);

/**
 * 判断 4字节 UTF-8 字符是否为可见字符
 * @param codepoint Unicode 代码点
 * @return true 如果是 4字节可见字符，false 否则
 */
bool is_visible_4byte_char(uint32_t codepoint);

/**
 * 获取 UTF-8 字符的类型
 * @param data UTF-8 字节序列
 * @param len 字节序列长度
 * @return UTF-8 字符类型
 */
utf8_char_type_t get_utf8_char_type(const uint8_t *data, size_t len);

/**
 * 过滤 UTF-8 字符串，保留指定类型的可见字符
 * @param input 输入字符串
 * @param input_len 输入字符串长度
 * @param output 输出缓冲区
 * @param output_size 输出缓冲区大小
 * @param options 过滤选项
 * @return 实际输出的字节数，失败返回 -1
 */
int filter_utf8_string(const uint8_t *input, size_t input_len,
                      uint8_t *output, size_t output_size,
                      const utf8_filter_options_t *options);

/**
 * 统计不同字节长度的可见字符数量
 * @param input 输入字符串
 * @param input_len 输入字符串长度
 * @param count_1byte 输出：1字节字符数量
 * @param count_2byte 输出：2字节字符数量
 * @param count_3byte 输出：3字节字符数量
 * @param count_4byte 输出：4字节字符数量
 * @return 总可见字符数，失败返回 -1
 */
int count_visible_chars_by_type(const uint8_t *input, size_t input_len,
                               int *count_1byte, int *count_2byte,
                               int *count_3byte, int *count_4byte);

/**
 * 打印字符的详细信息
 * @param data UTF-8 字节序列
 * @param len 字节序列长度
 */
void print_char_info(const uint8_t *data, size_t len);

#endif /* UTF8_FILTER_H */ 
