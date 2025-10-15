/*************************************************************************
> FileName: glob-match.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月15日 星期三 19时39分18秒
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>

bool glob_match(char const *pat, char const *str)
{
    char const *back_pat = NULL, *back_str;

    for (;;) {
        unsigned char c = *str++;
        unsigned char d = *pat++;

        switch (d) {
        case '?':   /* Wildcard: anything but nul */
            if (c == '\0')
                return false;
            break;
        case '*':   /* Any-length wildcard */
            if (*pat == '\0')   /* Optimize trailing * case */
                return true;
            back_pat = pat;
            back_str = --str;   /* Allow zero-length match */
            break;
        case '[': { /* Character class */
            if (c == '\0')  /* No possible match */
                return false;
            bool match = false, inverted = (*pat == '!');
            char const *class = pat + inverted;
            unsigned char a = *class++;

            /*
             * Iterate over each span in the character class.
             * A span is either a single character a, or a
             * range a-b.  The first span may begin with ']'.
             */
            do {
                unsigned char b = a;

                if (a == '\0')  /* Malformed */
                    goto literal;

                if (class[0] == '-' && class[1] != ']') {
                    b = class[1];

                    if (b == '\0')
                        goto literal;

                    class += 2;
                    /* Any special action if a > b? */
                }
                match |= (a <= c && c <= b);
            } while ((a = *class++) != ']');

            if (match == inverted)
                goto backtrack;
            pat = class;
            }
            break;
        case '\\':
            d = *pat++;
        default:    /* Literal character */
literal:
            if (c == d) {
                if (d == '\0')
                    return true;
                break;
            }
backtrack:
            if (c == '\0' || !back_pat)
                return false;   /* No point continuing */
            /* Try again from last *, one character later in str. */
            pat = back_pat;
            str = ++back_str;
            break;
        }
    }
}



int main (int argc, char* argv[])
{
    {
        const char* str = "aaa.txt";
        const char* patStr = "*.txt";
        printf ("'%s' -> '%s' --> %s\n", str, patStr, glob_match(patStr, str) ? "OK" : "No");
    }

    {
        const char* str = "aaa.txt1";
        const char* patStr = "*.txt";
        printf ("'%s' -> '%s' --> %s\n", str, patStr, glob_match(patStr, str) ? "OK" : "No");
    }

    {
        const char* str = "aaa.txt1";
        const char* patStr = "*.txt*";
        printf ("'%s' -> '%s' --> %s\n", str, patStr, glob_match(patStr, str) ? "OK" : "No");
    }

    return 0;
}
