/*************************************************************************
> FileName: struct-size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月20日 星期一 09时20分03秒
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>

typedef struct 
{
    uint32_t                        magic;
    uint32_t                        magicID;
    uint16_t                        version;
    uint16_t                        headSize;
    uint16_t                        fileType;
} S;


int main ()
{
    S s;
    printf("%d\n", sizeof(s));
    return 0;
}
