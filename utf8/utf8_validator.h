#ifndef UTF8_VALIDATOR_H
#define UTF8_VALIDATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * UTF-8 验证结果枚举
 */
typedef enum {
    UTF8_VALID,         // 完整有效的 UTF-8 字符
    UTF8_INCOMPLETE,    // 不完整的 UTF-8 字符（需要更多字节）
    UTF8_INVALID        // 无效的 UTF-8 序列
} utf8_result_t;

/**
 * 判断单个字节是否是 UTF-8 序列的开始字节
 * @param byte 要检查的字节
 * @return true 如果是开始字节，false 否则
 */
bool is_utf8_start_byte(uint8_t byte);

/**
 * 判断单个字节是否是 UTF-8 序列的继续字节
 * @param byte 要检查的字节
 * @return true 如果是继续字节，false 否则
 */
bool is_utf8_continuation_byte(uint8_t byte);

/**
 * 获取 UTF-8 字符的期望长度（基于第一个字节）
 * @param first_byte UTF-8 序列的第一个字节
 * @return 字符的期望字节长度，如果无效则返回 0
 */
int get_utf8_char_length(uint8_t first_byte);

/**
 * 验证 UTF-8 字符序列是否完整且有效
 * @param data 指向 UTF-8 字节序列的指针
 * @param len 可用的字节数
 * @param consumed 输出参数：实际消耗的字节数
 * @return UTF-8 验证结果
 */
utf8_result_t validate_utf8_char(const uint8_t *data, size_t len, size_t *consumed);

/**
 * 验证整个字符串是否为有效的 UTF-8
 * @param data 指向字节序列的指针
 * @param len 字节序列长度
 * @return true 如果整个序列都是有效的 UTF-8，false 否则
 */
bool is_valid_utf8_string(const uint8_t *data, size_t len);

/**
 * 计算 UTF-8 字符串中的字符数（非字节数）
 * @param data 指向 UTF-8 字节序列的指针
 * @param len 字节序列长度
 * @return 字符数，如果序列无效则返回 -1
 */
int count_utf8_chars(const uint8_t *data, size_t len);

#endif /* UTF8_VALIDATOR_H */ 