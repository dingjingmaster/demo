#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include "filter.h"

/**
 * 打印字符统计信息
 */
void print_char_statistics(const char *description, const uint8_t *data, size_t len) {
    printf("\n=== %s ===\n", description);
    printf("原始字符串: \"%.*s\"\n", (int)len, (char*)data);
    printf("字节长度: %zu\n", len);
    
    int count_1byte, count_2byte, count_3byte, count_4byte;
    int total = count_visible_chars_by_type(data, len, 
                                          &count_1byte, &count_2byte, 
                                          &count_3byte, &count_4byte);
    
    printf("字符统计:\n");
    printf("  1字节可见字符: %d\n", count_1byte);
    printf("  2字节可见字符: %d\n", count_2byte);
    printf("  3字节可见字符: %d\n", count_3byte);
    printf("  4字节可见字符: %d\n", count_4byte);
    printf("  总可见字符数: %d\n", total);
}

/**
 * 演示字符过滤功能
 */
void demo_character_filtering(const char *test_string) {
    const uint8_t *input = (const uint8_t*)test_string;
    size_t input_len = strlen(test_string);
    uint8_t output[1024];
    
    print_char_statistics("原始字符串分析", input, input_len);
    
    // 过滤选项
    utf8_filter_options_t options;
    
    // 只保留 1字节字符（ASCII）
    printf("\n--- 只保留 1字节字符 (ASCII) ---\n");
    memset(&options, 0, sizeof(options));
    options.include_1byte = true;
    int result_len = filter_utf8_string(input, input_len, output, sizeof(output), &options);
    if (result_len >= 0) {
        output[result_len] = '\0';
        printf("过滤结果: \"%s\"\n", (char*)output);
        printf("字节长度: %d\n", result_len);
    }
    
    // 只保留 2字节字符
    printf("\n--- 只保留 2字节字符 ---\n");
    memset(&options, 0, sizeof(options));
    options.include_2byte = true;
    result_len = filter_utf8_string(input, input_len, output, sizeof(output), &options);
    if (result_len >= 0) {
        output[result_len] = '\0';
        printf("过滤结果: \"%s\"\n", (char*)output);
        printf("字节长度: %d\n", result_len);
    }
    
    // 只保留 3字节字符
    printf("\n--- 只保留 3字节字符 ---\n");
    memset(&options, 0, sizeof(options));
    options.include_3byte = true;
    result_len = filter_utf8_string(input, input_len, output, sizeof(output), &options);
    if (result_len >= 0) {
        output[result_len] = '\0';
        printf("过滤结果: \"%s\"\n", (char*)output);
        printf("字节长度: %d\n", result_len);
    }
    
    // 只保留 4字节字符
    printf("\n--- 只保留 4字节字符 ---\n");
    memset(&options, 0, sizeof(options));
    options.include_4byte = true;
    result_len = filter_utf8_string(input, input_len, output, sizeof(output), &options);
    if (result_len >= 0) {
        output[result_len] = '\0';
        printf("过滤结果: \"%s\"\n", (char*)output);
        printf("字节长度: %d\n", result_len);
    }
    
    // 保留所有可见字符
    printf("\n--- 保留所有可见字符 ---\n");
    memset(&options, 0, sizeof(options));
    options.include_1byte = true;
    options.include_2byte = true;
    options.include_3byte = true;
    options.include_4byte = true;
    result_len = filter_utf8_string(input, input_len, output, sizeof(output), &options);
    if (result_len >= 0) {
        output[result_len] = '\0';
        printf("过滤结果: \"%s\"\n", (char*)output);
        printf("字节长度: %d\n", result_len);
    }
}

/**
 * 演示各字节长度字符范围
 */
void demo_char_ranges() {
    printf("\n=*60\n");
    printf("UTF-8 各字节长度可见字符范围演示\n");
    printf("=*60 \n");
    
    // 2字节字符示例
    printf("\n=== 2字节 UTF-8 可见字符示例 ===\n");
    const char *samples_2byte[] = {
        "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É",  // 拉丁扩展
        "Α", "Β", "Γ", "Δ", "Ε", "Ζ", "Η", "Θ", "Ι", "Κ",  // 希腊文
        "А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И",  // 西里尔文
        "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט", "י",  // 希伯来文
        "ا", "ب", "ت", "ث", "ج", "ح", "خ", "د", "ذ", "ر"   // 阿拉伯文
    };
    
    for (size_t i = 0; i < sizeof(samples_2byte)/sizeof(samples_2byte[0]); i++) {
        const uint8_t *char_data = (const uint8_t*)samples_2byte[i];
        size_t char_len = strlen(samples_2byte[i]);
        uint32_t codepoint = utf8_to_codepoint(char_data, char_len);
        printf("'%s' (U+%04X) ", samples_2byte[i], codepoint);
        if ((i + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
    
    // 3字节字符示例
    printf("\n=== 3字节 UTF-8 可见字符示例 ===\n");
    const char *samples_3byte[] = {
        "中", "国", "人", "民", "共", "和", "你", "好", "世", "界",  // 中文
        "日", "本", "語", "文", "字", "漢", "書", "学", "校", "生",  // 日文汉字
        "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ",  // 平假名
        "ア", "イ", "ウ", "エ", "オ", "カ", "キ", "ク", "ケ", "コ",  // 片假名
        "한", "글", "문", "자", "대", "한", "민", "국", "안", "녕"   // 韩文
    };
    
    for (size_t i = 0; i < sizeof(samples_3byte)/sizeof(samples_3byte[0]); i++) {
        const uint8_t *char_data = (const uint8_t*)samples_3byte[i];
        size_t char_len = strlen(samples_3byte[i]);
        uint32_t codepoint = utf8_to_codepoint(char_data, char_len);
        printf("'%s' (U+%04X) ", samples_3byte[i], codepoint);
        if ((i + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
    
    // 4字节字符示例（表情符号）
    printf("\n=== 4字节 UTF-8 可见字符示例 ===\n");
    const char *samples_4byte[] = {
        "😀", "😁", "😂", "😃", "😄", "😅", "😆", "😇", "😈", "😉",  // 表情符号
        "🌍", "🌎", "🌏", "🌐", "🌑", "🌒", "🌓", "🌔", "🌕", "🌖",  // 地球和月亮
        "🚀", "🚁", "🚂", "🚃", "🚄", "🚅", "🚆", "🚇", "🚈", "🚉",  // 交通工具
        "🎵", "🎶", "🎸", "🎹", "🎺", "🎻", "🎼", "🎤", "🎧", "🎨"   // 音乐和艺术
    };
    
    for (size_t i = 0; i < sizeof(samples_4byte)/sizeof(samples_4byte[0]); i++) {
        const uint8_t *char_data = (const uint8_t*)samples_4byte[i];
        size_t char_len = strlen(samples_4byte[i]);
        uint32_t codepoint = utf8_to_codepoint(char_data, char_len);
        printf("'%s' (U+%05X) ", samples_4byte[i], codepoint);
        if ((i + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
}

/**
 * 逐字符分析演示
 */
void demo_char_analysis(const char *text) {
    printf("\n  =*60 \n");
    printf("逐字符详细分析: \"%s\"\n", text);
    printf("=*60 \n");
    
    const uint8_t *data = (const uint8_t*)text;
    size_t len = strlen(text);
    size_t pos = 0;
    int char_num = 1;
    
    while (pos < len) {
        size_t consumed = 0;
        utf8_result_t result = validate_utf8_char(data + pos, len - pos, &consumed);
        
        if (result == UTF8_VALID) {
            printf("\n--- 字符 %d (位置 %zu) ---\n", char_num++, pos);
            print_char_info(data + pos, consumed);
            pos += consumed;
        } else {
            printf("\n--- 无效字节 (位置 %zu) ---\n", pos);
            printf("字节: 0x%02X\n", data[pos]);
            pos++;
        }
    }
}

int main() {
    // 设置本地化
    setlocale(LC_ALL, "");
    
    printf("UTF-8 字符过滤器演示程序\n");
    printf("=======================\n");
    
    // 演示字符范围
    demo_char_ranges();
    
    // 测试字符串
    const char *test_strings[] = {
        "Hello 世界 🌍",                    // 混合字符串
        "ABC中文123",                       // ASCII + 中文 + 数字
        "Café résumé naïve Zürich",       // 拉丁扩展字符
        "Привет мир Москва",              // 西里尔文
        "مرحبا بالعالم العربي",            // 阿拉伯文  
        "こんにちは世界カタカナ",            // 日文（平假名+汉字+片假名）
        "안녕하세요 한글 세계",             // 韩文
        "Programming 编程 🚀💻🎯",         // 技术主题混合
        "Math: ∑∫∞ Greek: αβγ Emoji: 😊🌟⭐" // 数学符号+希腊字母+表情
    };
    
    // 对每个测试字符串进行过滤演示
    for (size_t i = 0; i < sizeof(test_strings)/sizeof(test_strings[0]); i++) {
        printf("\n =*80 \n");
        printf("测试字符串 %zu", i + 1);
        printf("\n =*80 \n");
        demo_character_filtering(test_strings[i]);
    }
    
    // 详细字符分析演示
    demo_char_analysis("Hi中🌍");
    
    printf("\n =*60 \n");
    printf("UTF-8 字节长度与 Unicode 范围对照表\n");
    printf("=*60 \n");
    printf("1字节: U+0000 - U+007F   (ASCII, 0-127)\n");
    printf("  可见范围: U+0020 - U+007E (空格到~)\n");
    printf("  示例: A-Z, a-z, 0-9, !@#$%%^&*()\n\n");
    
    printf("2字节: U+0080 - U+07FF   (128-2047)\n");
    printf("  主要包含: 拉丁扩展、希腊文、西里尔文、希伯来文、阿拉伯文\n");
    printf("  示例: À, Á, Â, Α, Β, Γ, А, Б, В, א, ב, ג, ا, ب, ت\n\n");
    
    printf("3字节: U+0800 - U+FFFF   (2048-65535)\n");
    printf("  主要包含: 中日韩文字、符号、标点、数学符号\n");
    printf("  示例: 中文汉字、日文假名、韩文、各种符号\n\n");
    
    printf("4字节: U+10000 - U+10FFFF (65536-1114111)\n");
    printf("  主要包含: 表情符号、扩展汉字、古文字\n");
    printf("  示例: 😀, 🌍, 🚀, 扩展汉字区域\n\n");
    
    return 0;
} 
