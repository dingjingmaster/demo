/*************************************************************************
> FileName: struct-align.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Mar 2024 09:59:25 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief
 *  1. 第一个成员首地址为0；
 *  2. 每个成员的首地址是自身大小的整数倍；
 *  3. 结构体中大小，是成员中所含最大类型的整数倍；
 */

// 16
typedef struct _S1 
{
    double  s1;         // 0 - 7
    char    s2;         // 8 - 9
} S1;                   // = 16

// 24
typedef struct _S2 
{
    double  s1;         // 0 - 7
    char    s2[10];     // 8 - 17
    char    s3;         // 18 - 19
} S2;                   // = 24

// 32
typedef struct _S3 
{
    double  s1;         // 0 - 7
    char    s2[10];     // 8 - 17
    char    s3[8];      // 18 - 26
} S3;                   // = 32

// 24
typedef struct _S4
{
    double  s1;         // 0 - 7
    char    s2[10];     // 8 - 17
    char    s3[6];      // 18
} S4;                   // = 24

// 16
typedef struct _S5
{
    double  s1;         // 0 - 7
    char    s2;         // 8
    char    s3;         // 9
} S5;                   // = 16

// 16
typedef struct _S6
{
    double  s1;         // 0 - 7
    char    s2;         // 8
    char    s3;         // 9
    char    s4;         // 10
    char    s5;         // 11
    char    s6;         // 12
    char    s7;         // 13
    char    s8;         // 14
    char    s9;         // 15
} S6;                   // = 16

// 16
typedef struct _S7
{
    double  s1;         // 0 - 7
    char    s2;         // 8
    char    s3;         // 9
    char    s4;         // 10
    char    s5;         // 11
    int     s6;         // 12 - 15
} S7;                   // = 16

// 24
typedef struct _S8
{
    double  s1;         // 0 - 7
    char    s2;         // 8
    int     s6;         // 12 - 15
    char    s3;         // 16
} S8;                   // = 24


// 40
typedef struct _S9
{
    double  s1;             // 0 - 7
    char    s2;             // 8
    struct _S21             // >>> 8倍
    {
        double  ss1;        // 16 - 23
        char    ss2[10];    // 24 - 34
    } s3;                   
} S9;                       // = 40

// 32
typedef struct _S10
{
    double  s1;             // 0 - 7
    char    s2;             // 8
    struct                  // >>> 4倍
    {
        int     ss1;        // 12 - 15
        char    ss2[10];    // 15 - 24
    } S21;                   
} S10;                      // = 32

// 24
typedef struct _S11
{
    double  s1;             // 0 - 7
    char    s2;             // 8
    struct                  // >>> 2倍
    {
        short   ss1;        // 10 - 11
        char    ss2[10];    // 12 - 21
    } S21;                   
} S11;                      // = 24


// 8
typedef union _S12
{
    double  u1;             // 0 - 7
    char    u2;             // 0
    int     u3;             // 0 - 3
} S12;                      // = 8

// 8
typedef union _S13
{
    char    u1;             // 0
    struct {                // >> 4 倍
        int     u21;        // 0 - 4
        short   u22;        // 5 - 6
    } u2;
} S13;                      // =8


int main ()
{
    printf ("char   : %lu\n", sizeof(char));        // 1    8bit
    printf ("short  : %lu\n", sizeof(short));       // 2    16bit
    printf ("int    : %lu\n", sizeof(int));         // 4    32bit
    printf ("long   : %lu\n", sizeof(long));        // 8    64bit
    printf ("float  : %lu\n", sizeof(float));       // 4    32bit
    printf ("double : %lu\n", sizeof(double));      // 8    64bit

    printf ("S1     : %lu\n", sizeof(S1));          // 16   128bit
    printf ("S2     : %lu\n", sizeof(S2));          // 24
    printf ("S3     : %lu\n", sizeof(S3));          // 32
    printf ("S4     : %lu\n", sizeof(S4));          // 24
    printf ("S5     : %lu\n", sizeof(S5));          // 16
    printf ("S6     : %lu\n", sizeof(S6));          // 16
    printf ("S7     : %lu\n", sizeof(S7));          // 16
    printf ("S8     : %lu\n", sizeof(S8));          // 24
    printf ("S9     : %lu\n", sizeof(S9));          // 40
    printf ("S10    : %lu\n", sizeof(S10));         // 32
    printf ("S11    : %lu\n", sizeof(S11));         // 24
    printf ("S12    : %lu\n", sizeof(S12));         // 8
    printf ("S13    : %lu\n", sizeof(S13));         // 8

    return 0;
}
