#include <stdio.h>

/**
 * Bit Fields
 *
 * Here, the packed_struct contains 6 members:
 * Four 1 bit flags f1..f3, a 4-bit type and a 9-bit my_int.
 *
 * use example:
 *  1. Packing several objects into a machine word.
 *       e.g. 1 bit flags can be compacted.
 *  2. Reading external file formats -- non-standard file formats
 *       could be read in, e.g., 9-bit integers.
 *
 * construction:
 *  C automatically packs the above bit fields as compactly as possible,
 *  provided that the maximum length of the field is less than or equal to
 *  the integer word length of the computer. If this is not the case,
 *  then some compilers may allow memory overlap for the fields while
 *  others would store the next field in the next word.
 */
struct packed_struct {
   unsigned int f1:1;       // int is 4 byte(32 bit in 32bit system), 1 means bit fields length, f1 use 1 bit
   unsigned int f2:1;
   unsigned int f3:1;
   unsigned int f4:1;
   unsigned int type:4;
   unsigned int my_int:9;
} pack;                     // unsigned int is 32 bit. so that can't store than 32 bit

struct ks {
    char a:1;
    char :2;                // this 2 bit can't use because of no name
    char b:3;
    char c:8;
    char d:6;
    char e:6;
    char f:6;
    char g:6;
}k; // 38bit total, can't over 4byte + 2 byte


int main() {

    pack.f1 = 0x1;

    printf("f1: %d\n", pack.f1);
    printf("k: %d\n", sizeof(k));               // result: 6 (max 4 + 2)
    printf("pack: %d\n", sizeof(pack));         // result: 4
    printf("int: %d\n", sizeof(int));           // result: 4
    printf("char: %d\n", sizeof(char));         // result: 1

    return 0;
}
