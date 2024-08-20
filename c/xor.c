/*************************************************************************
> FileName: xor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Aug 2024 09:25:21 AM CST
 ************************************************************************/
#include <stdio.h>

/**
 * 1. 按位异或 等价于 不进位 加法 ？？
 * 2. 对一个数据做两次按位异或运算，可以将该数据反转回原值
 * 3. 交换运算
 * 4. 奇偶性判断
 */
int main (int argc, char* argv[])
{
    int ret = 1^1; 
    printf("%u(%0x)\n", ret, ret); // 0 == 0x01 ^ 0x01

    ret = 1^2;
    printf("%u(%0x)\n", ret, ret); // 3 == 0x01 ^ 0x10

    ret = 1^3;
    printf("%u(%0x)\n", ret, ret); // 2 == 0x01 ^ 0x11

    ret = 1^4;
    printf("%u(%0x)\n", ret, ret); // 5 == 0x001 ^ 0x100 = 0x101

    ret = 1^5;
    printf("%u(%0x)\n", ret, ret); // 4 == 0x001 ^ 0x101 = 0x100

    ret = 1^6;
    printf("%u(%0x)\n", ret, ret); // 7 == 0x001 ^ 0x110 = 0x111

    ret = 1^7;
    printf("%u(%0x)\n", ret, ret); // 6 == 0x0001 ^ 0x0111 = 0x0110

    return 0;
}
