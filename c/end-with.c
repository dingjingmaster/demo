/*************************************************************************
> FileName: end-with.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月03日 星期一 09时40分58秒
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>


static inline bool end_with (const char* str, const char* suffix)
{
    int len1 = 0;
    int len2 = 0;
    int idx1 = 0;
    int idx2 = 0;

    if (!str || !suffix) {
        return false;
    }


    for (len1 = 0; str[len1] != '\0'; ++len1);
    for (len2 = 0; suffix[len2] != '\0'; ++len2);

    if (len2 > len1) {
        return false;
    }

    idx1 = len1 - len2;
    printf ("start idx1: %d\nstart idx2: %d\nlen1: %d\nlen2: %d\n", idx1, idx2, len1, len2);
    for (idx1 = len1 - len2, idx2 = 0; (str[idx1] != '\0' && suffix[idx2] != '\0') && (str[idx1] == suffix[idx2]); ++idx1,++idx2);

    printf ("\nend idx1: %d\nend idx2: %d\nlen1: %d\nlen2: %d\n", idx1, idx2, len1, len2);

    if (idx1 == len1 && idx2 == len2) { return true; }

    return false;
}


int main (int argc, char* argv[])
{
    if (argc != 3) {
        printf ("Please input: %s <string> <suffix string>\n", argv[0]);
        return -1;
    }

    printf ("%s => %s, res=%s\n", argv[1], argv[2], (end_with(argv[1], argv[2]) ? "true" : "false"));

    return 0;
}

