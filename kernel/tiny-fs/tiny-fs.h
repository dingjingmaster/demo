/*************************************************************************
> FileName: tiny-fs.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 09 May 2024 09:35:08 AM CST
 ************************************************************************/
#ifndef _TINY_FS_H
#define _TINY_FS_H

// tinyfs.h
#define MAXLEN 8
#define MAX_FILES    32
#define MAX_BLOCKSIZE  512

// 定义每一个目录项的格式
struct dir_entry {
    char filename[MAXLEN];
    uint8_t idx;
};

// 定义每一个文件的格式。
struct file_blk {
    uint8_t busy;
    mode_t mode;
    uint8_t idx;

    union {
        uint8_t file_size;
        uint8_t dir_children;
    };
    char data[0];
};

// OK，下面的block数组所占据的连续内存就是我的tinyfs的介质，每一个元素代表一个文件。
// struct file_blk block[MAX_FILES+1];

#endif
