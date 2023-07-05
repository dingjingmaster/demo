#ifndef C_LOG_H
#define C_LOG_H

/**
 * full_path_name = _log_path/_log_name.log
 * _log_path = /path/of/file
 * _log_name = _log_name_prefix _log_name_time ._log_name_suffix
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef LOG_TAG
#define LOG_TAG "UNKNOWN"
#endif

typedef enum {
    LOG_TYPE_FILE       =   0,              /* 输出日志到文件 */
    LOG_TYPE_CONSOLE,                       /* 输出到控制台 */
} log_type_t;

typedef enum {
    LOG_EMERG           =   0,              /* 系统不兼容 */
    LOG_ALERT           =   1,              /* 严重错误 */
    LOG_CRIT            =   2,              /* 严重警告 */
    LOG_ERR             =   3,              /* 错误 */
    LOG_WARNING         =   4,              /* 警告 */
    LOG_NOTICE          =   5,              /* 正常但重要的情况 */
    LOG_INFO            =   6,              /* 信息 */
    LOG_DEBUG           =   7,              /* 调试信息 */
    LOG_VERB            =   8,              /* 详细信息 */
} log_level_t;

typedef enum {
    LOG_ROTATE_TRUE     =   1,              /* 允许分文件 */
    LOG_ROTATE_FALSE    =   2,              /* 不允许分文件 */
} log_rotate_t;


/**
 *
 * 初始化 log 参数
 *
 * @param level: 设置 log 输出级别
 * @param rotate: 是否切分文件
 * @param log_size: 每个日志文件的大小
 * @param dir: 日志文件存储文件夹路径
 * @param prefix: 日志文件名
 * @param suffix: 日志文件后缀名
 *
 * @return 成功: 0; 失败: -1
 */
int log_init(log_type_t type, log_level_t level, log_rotate_t rotate, unsigned long long log_size,
        const char* dir, const char* prefix, const char* suffix);

/**
 * 销毁 log 参数
 *
 */
void log_destroy(void);

/**
 *
 * 输出日志信息到文件
 *
 */
int log_print(log_level_t level, const char* tag, const char* file, int line, const char* func, const char* fmt, ...);
#define loge(...) log_print(LOG_ERR, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define logw(...) log_print(LOG_WARNING, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define logi(...) log_print(LOG_INFO, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)

#if DEBUG
#define logd(...) log_print(LOG_DEBUG, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define logv(...) log_print(LOG_VERB, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define logd(...)
#define logv(...)
#endif

#ifdef __cplusplus
}
#endif

#endif // C_LOG_H
