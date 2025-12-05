/*************************************************************************
> FileName: str-match.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年12月05日 星期五 18时03分08秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool utils_str_match (char const* pat, char const* str)
{
    char* class = NULL;
    bool match = false;
    bool inverted = false;
    unsigned char a = 0, b = 0;
    char const *backPat = NULL, *backStr = NULL;

    for (;;) {
        unsigned char c = *str++;
        unsigned char d = *pat++;
        if (c >= 'A' && c <= 'Z') { c += 32; }
        if (d >= 'A' && d <= 'Z') { d += 32; }
        switch (d) {
            case '?': {   /* Wildcard: anything but nul */
                if (c == '\0') {
                    return false;
                }
                break;
            }
            case '*': {  /* Any-length wildcard */
                if (*pat == '\0') {  /* Optimize trailing * case */
                    return true;
                }
                backPat = pat;
                backStr = --str;   /* Allow zero-length match */
                break;
            }
            case '[': { /* Character class */
                if (c == '\0') { /* No possible match */
                    return false;
                }
                match = false;
                inverted = (*pat == '!');
                class = (char*) pat + inverted;
                a = *class++;
                /**
                 * Iterate over each span in the character class.
                 * A span is either a single character a, or a
                 * range a-b.  The first span may begin with ']'.
                 */
                do {
                    b = a;
                    if (a == '\0') { /* Malformed */
                        goto literal;
                    }
                    if (class[0] == '-' && class[1] != ']') {
                        b = class[1];
                        if (b == '\0') {
                            goto literal;
                        }
                        class += 2;
                        /* Any special action if a > b? */
                    }
                    match |= (a <= c && c <= b);
                } while ((a = *class++) != ']');
                if (match == inverted) {
                    goto backtrack;
                }
                pat = class;
                break;
            }
            case '\\': {
                d = *pat++;
                /* fall through */
            }
            default: {   /* Literal character */
literal:
                if (c == d) {
                    if (d == '\0') {
                        return true;
                    }
                    break;
                }
backtrack:
                if (c == '\0' || !backPat) {
                    return false;   /* No point continuing */
                }
                /* Try again from last *, one character later in str. */
                pat = backPat;
                str = ++backStr;
                break;
            }
        }
    }


    return false;
}

int main (int argc, char* argv[])
{
    if (argc != 3) {
        printf ("Please input: %s <string> <*.suffix string>\n", argv[0]);
        return -1;
    }

    printf ("%s => %s, res=%s\n", argv[1], argv[2], (utils_str_match(argv[2], argv[1]) ? "true" : "false"));



    return 0;
}
