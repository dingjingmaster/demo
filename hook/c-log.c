#include "c-log.h"

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_IOVEC_MAX               16
#define LOG_FILENAME_LEN            (32)
#define LOG_DIRNAME_LEN             (1024)
#define LOG_PATH_MAX                (2048)
#define LOG_BUF_SIZE                (20480)
#define PATH_SPLIT                  '/'

static const char* _log_level_str[] = {
    "EMERG",
    "ALERT",
    "CRIT",
    "ERROR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
    "VERBOSE",
    NULL
};

static log_type_t _log_type = LOG_TYPE_CONSOLE;                         /* 日志默认输出到控制台 */
static log_rotate_t _log_rotate;                                        /* 是否允许分文件 */
static unsigned long long _log_size = 0;                                /* 日志文件大小 */
static log_level_t _log_level;                                          /* 输出日至级别 */
static char _log_dir[LOG_DIRNAME_LEN] = "./";                           /* 日志输出文件夹 */
static char _log_prefix[LOG_FILENAME_LEN] = "log";                      /* 日志名称 */
static char _log_suffix[LOG_FILENAME_LEN] = "log";                      /* 日志扩展名 */
static char _path_name[LOG_PATH_MAX] = {0};                             /* 完整日志路径 */
static int _log_fd = 0;                                                 /* 当前打开的日志文件描述符 */


static pthread_mutex_t _log_mutex;                                      /* 日志锁 */
static pthread_once_t _thread_once = PTHREAD_ONCE_INIT;                 /* 确保初始化一次 */

static int _is_log_init = 0;                                            /* 是否完成初始化 */


#define FG_BLACK                    30
#define FG_RED                      31
#define FG_GREEN                    32
#define FG_YELLOW                   33
#define FG_BLUE                     34
#define FG_MAGENTA                  35
#define FG_CYAN                     36
#define FG_WHITE                    37
#define BG_BLACK                    40
#define BG_RED                      41
#define BG_GREEN                    42
#define BG_YELLOW                   43
#define BG_BLUE                     44
#define BG_MAGENTA                  45
#define BG_CYAN                     46
#define BG_WHITE                    47
#define B_RED(str)                  "\033[1;31m" str "\033[0m"
#define B_GREEN(str)                "\033[1;32m" str "\033[0m"
#define B_YELLOW(str)               "\033[1;33m" str "\033[0m"
#define B_BLUE(str)                 "\033[1;34m" str "\033[0m"
#define B_MAGENTA(str)              "\033[1;35m" str "\033[0m"
#define B_CYAN(str)                 "\033[1;36m" str "\033[0m"
#define B_WHITE(str)                "\033[1;37m" str "\033[0m"
#define RED(str)                    "\033[31m" str "\033[0m"
#define GREEN(str)                  "\033[32m" str "\033[0m"
#define YELLOW(str)                 "\033[33m" str "\033[0m"
#define BLUE(str)                   "\033[34m" str "\033[0m"
#define MAGENTA(str)                "\033[35m" str "\033[0m"
#define CYAN(str)                   "\033[36m" str "\033[0m"
#define WHITE(str)                  "\033[37m" str "\033[0m"


static const char* get_dir(const char* path)
{
    char* p = (char*)path + strlen(path);
    for(; p != path; p--) {
        if('/' == *p) {
            *(p + 1) = '\0';
            break;
        }
    }
    return path;
}

static int mkdir_r(const char* path, mode_t mode)
{
    int ret = 0;
    char* temp = NULL;
    char* pos = NULL;
    if(NULL == path) {
        return -1;
    }
    temp = strdup(path);
    pos = temp;
    if(0 == strncmp(temp, "/", 1)) {
        pos += 1;
    } else if(0 == strncmp(temp, "./", 2)) {
        pos += 2;
    }
    for(; *pos != '\0'; ++ pos) {
        if(*pos == '/') {
            *pos = '\0';
            if (-1 == (ret = mkdir(temp, mode))) {
                if(errno == EEXIST) {
                    ret = 0;
                } else {
                    fprintf(stderr, "fail to mkdir %s: %d:%s\n",
                            temp, errno, strerror(errno));
                    break;
                }
            }
            *pos = '/';
        }
    }
    if(*(pos - 1) != '/') {
        if(-1 == (ret = mkdir(temp, mode))) {
            if(errno == EEXIST) {
                ret = 0;
            } else {
                fprintf(stderr, "failed to mkdir %s: %d:%s\n",
                        temp, errno, strerror(errno));
            }
        }
    }
    free(temp);
    return ret;
}

static int check_dir (const char* path)
{
    char* path_tmp = NULL;
    const char* dir = NULL;
    path_tmp = strdup(path);
    if(NULL != strstr(path, "/")) {
        dir = get_dir(path_tmp);
        if (-1 == access(dir, F_OK | W_OK | R_OK)) {
            if(-1 == mkdir_r(dir, 0776)) {
                fprintf(stderr, "mkdir %s failed\n", path_tmp);
                goto RET_ERR;
            }
        }
    }

    free(path_tmp);
    return 0;

RET_ERR:
    free(path_tmp);
    return -1;
}

static void _log_get_time(char* str, int len, int flag)
{
    char date_fmt[20] = {0};
    char date_ms[4] = {0};
    struct timeval tv;
    struct tm now_tm;
    long now_ms;
    time_t now_sec;
    gettimeofday(&tv, NULL);
    now_sec = tv.tv_sec;
    now_ms = tv.tv_usec/1000;
    localtime_r(&now_sec, &now_tm);

    if (0 == flag) {
        strftime(date_fmt, 20, "%Y-%m-%d %H:%M:%S", &now_tm);
        snprintf(date_ms, sizeof(date_ms), "%03ld", now_ms);
        snprintf(str, (unsigned long)len, "[%s.%s]", date_fmt, date_ms);
    } else {
        strftime(date_fmt, 20, "%Y_%m_%d", &now_tm);
        snprintf(str, (unsigned long)len, "%s", date_fmt);
    }
}

static int _open_file()
{
    if(0 != check_dir(_log_dir)) {
        fprintf(stderr, "check_dir error, log_init failed\n");
        return -1;
    }
    //char time_str[LOG_FILENAME_LEN] = {0};
    //_log_get_time(time_str, sizeof(time_str), 1);
    if(0 >= snprintf(_path_name, sizeof(_path_name), "%s/%s.%s",
            _log_dir, _log_prefix, _log_suffix)) {
        return -1;
    }
    _log_fd = open(_path_name, O_CREAT | O_RDWR | O_APPEND, 0664);
    if(-1 == _log_fd) {
        fprintf(stderr, "open %s error: %s, log_init failed\n",
                _path_name, strerror(errno));
        _log_fd = STDERR_FILENO;
        return -1;
    }
    return 0;
}

static void log_init_once(void)
{
    if(1 == _is_log_init) {
        return;
    }
    _is_log_init = 1;
    pthread_mutex_init(&_log_mutex, NULL);
}

int log_init(log_type_t type, log_level_t level, log_rotate_t rotate, unsigned long long log_size, const char *dir, const char *prefix, const char *suffix)
{
    if (0 != pthread_once(&_thread_once, log_init_once)) {
        fprintf(stderr, "pthread_once error, log_init failed\n");
        return -1;
    }

    if(0 != pthread_mutex_lock(&_log_mutex)) {
        fprintf(stderr, "pthread_mutex_lock error, log_init failed\n");
        return -1;
    }

    _log_type = type;
    _log_level = level;
    _log_rotate = rotate;
    if (log_size > 0) {
        _log_size = log_size;
    } else {
        _log_size = 2 << 30;
    }
    if (NULL != dir) {
        unsigned long dl = strlen(dir);
        if(dl > 2) {
            if(0 == strncmp(dir, "./", 2) || 0 == strncmp(dir, "/", 1)) {
                snprintf(_log_dir, sizeof(_log_dir) - 2, "%s/", dir);
            } else {
                snprintf(_log_dir, sizeof(_log_dir) - 4, "./%s/", dir);
            }
        } else {
            if (0 == strncmp(dir, "/", 1)) {
                strncpy(_log_dir, dir, sizeof(_log_dir) - 1);
            } else {
                fprintf(stderr, "dir name is invalid!\n");
            }
        }
    }
    if (NULL != prefix) {
        strncpy(_log_prefix, prefix, sizeof(_log_prefix));
    }
    if (NULL != suffix) {
        strncpy(_log_suffix, suffix, sizeof(_log_suffix));
    }

    if(LOG_TYPE_CONSOLE == _log_type) {
        _log_fd = STDOUT_FILENO;
    } else {
        if(0 != _open_file()) {
            goto RET_ERR;
        }
    }

    if(0 != pthread_mutex_unlock(&_log_mutex)) {
        fprintf(stderr, "pthread_mutex_unlock error, log_init failed\n");
        goto RET_ERR;
    }

    return 0;

RET_ERR:
    pthread_mutex_unlock(&_log_mutex);
    return -1;
}

void log_destroy(void)
{
    if(!_is_log_init) {
        return;
    }
    _is_log_init = 0;
    pthread_mutex_lock(&_log_mutex);
    close(_log_fd);
    _thread_once = PTHREAD_ONCE_INIT;
    pthread_mutex_unlock(&_log_mutex);
    pthread_mutex_destroy(&_log_mutex);
}

static unsigned long long get_file_size(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) < 0) {
        return 0;
    }
    return (unsigned long long)buf.st_size;
}

static int _log_open_rewrite(const char *path)
{
    check_dir(path);
    _log_fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (_log_fd == -1) {
        fprintf(stderr, "open %s failed: %s\n", path, strerror(errno));
        fprintf(stderr, "use STDERR_FILEIO as output\n");
        _log_fd = STDERR_FILENO;
    }
    return 0;
}

static ssize_t _log_write(struct iovec *vec, int n)
{
    unsigned long long tmp_size = get_file_size(_path_name);
    if (tmp_size > _log_size) {
        if (LOG_ROTATE_FALSE == _log_rotate) {
            if (-1 == close(_log_fd)) {
                fprintf(stderr, "close file errno:%d", errno);
            }
            _log_open_rewrite(_path_name);
        } else {
            if (-1 == close(_log_fd)) {
                fprintf(stderr, "close file errno:%d", errno);
            }
            if (-1 == _open_file()) {
                fprintf(stderr, "rotate fail errno:%d", errno);
            }
        }
    }

    return writev(_log_fd, vec, n);
}

static const char* _file_name(const char* path, int len)
{
    if (NULL == path) {
        return path;
    }
    char* pend = NULL;
    pend = (char*)path + len;
    int i = 0;
    for(i = 1; i < len; ++i) {
        if((pend - i == path) || (*(pend - i) == '/')
         || (*(pend - i) == '\\')) break;
    }
    if(i >= 2) i--;

    return pend - i;
}

static int _log_print(log_level_t level, const char* tag, const char* file, int line, const char* func, const char* msg)
{
    int ret = 0;
    struct iovec vec[LOG_IOVEC_MAX];
    char s_time[LOG_FILENAME_LEN] = {0};
    char s_level[LOG_FILENAME_LEN] = {0};
    char s_tag[LOG_FILENAME_LEN] = {0};
    char s_pid[LOG_FILENAME_LEN] = {0};
    char s_file[LOG_DIRNAME_LEN] = {0};
    char s_msg[LOG_BUF_SIZE] = {0};

    pthread_mutex_lock(&_log_mutex);
    _log_get_time(s_time, sizeof(s_time), 0);
    if(STDERR_FILENO == _log_fd || STDOUT_FILENO == _log_fd) {
        switch(level) {
        case LOG_EMERG:
        case LOG_ALERT:
        case LOG_CRIT:
        case LOG_ERR:
            snprintf(s_level, sizeof(s_level), B_RED("[%s] "), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), RED(" %s"), msg);
            break;
        case LOG_WARNING:
            snprintf(s_level, sizeof(s_level), B_YELLOW("[%s] "), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), YELLOW(" %s"), msg);
            break;
        case LOG_INFO:
            snprintf(s_level, sizeof(s_level), B_GREEN("[%s] "), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), GREEN(" %s"), msg);
            break;
        case LOG_DEBUG:
            snprintf(s_level, sizeof(s_level), B_WHITE("[%s] "), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), WHITE(" %s"), msg);
            break;
        default:
            snprintf(s_level, sizeof(s_level), "[%s] ", _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), " %s", msg);
            break;
        }
    } else {
        snprintf(s_level, sizeof(s_level), "[%s] ", _log_level_str[level]);
        snprintf(s_msg, sizeof(s_msg), " %s", NULL == msg ? "<null>":msg);
    }
    snprintf(s_tag, sizeof(s_tag), "[%s] ", tag);
    snprintf(s_pid, sizeof(s_pid), "[pid:%d ", getpid());
    snprintf(s_file, sizeof(s_file) - 1, "%s:%d: %s] ", _file_name(file, strlen(file)), line, func);

    int i = -1;
    vec[++i].iov_base = (void*)s_time;
    vec[i].iov_len = strlen(s_time);
    vec[++i].iov_base = " ";
    vec[i].iov_len = 1;
    vec[++i].iov_base = (void*)s_tag;
    vec[i].iov_len = strlen(s_tag);
    vec[++i].iov_base = (void*)s_level;
    vec[i].iov_len = strlen(s_level);
    vec[++i].iov_base = (void*)s_pid;
    vec[i].iov_len = strlen(s_pid);
    vec[++i].iov_base = (void*)s_file;
    vec[i].iov_len = strlen(s_file);
    vec[++i].iov_base = (void*)s_msg;
    vec[i].iov_len = strlen(s_msg);
    vec[++i].iov_base = "\n";
    vec[i].iov_len = 1;

    ret = (int)_log_write(vec, ++i);
    pthread_mutex_unlock(&_log_mutex);
    return ret;
}

int log_print(log_level_t level, const char *tag, const char *file, int line, const char *func, const char *fmt,...)
{
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int n;
    if (level > _log_level) {
        return 0;
    }

    if (1 != _is_log_init) {
        fprintf(stderr, "log has not been invalid!\n");
        return -1;
    }

    // FIXME:// 此处太多数据会导致崩溃
    va_start(ap, fmt);
    n = vsnprintf(buf, LOG_BUF_SIZE - 1, fmt, ap);
    va_end(ap);
    if (n < 0) {
        return -1;
    }

    return _log_print(level, tag, file, line, func, buf);
}
