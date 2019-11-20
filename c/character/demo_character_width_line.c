/*************************************************************************
> FileName: demo_character_width_line.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月18日 星期一 10时26分29秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>

#define CHARACTER_WIDTH 12

int is_ascii (char c) 
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57) || (c == 95))
        return 0;
    return 1;
}

// 仅支持 utf8
void string_length (char* str, int clen, char* str1, int str1len)
{
    int i = 0;
    int len = 0;
    int sumflag = 0;
    int str1nowlen = 0;
    short ischinese = 0;                  // 1 不是中文 2 中文
    
    memset(str1, 0, str1len);

    for (i = 0; i < strlen(str); ++i) {
        ischinese = is_ascii(str[i]) == 0 ? 1 : 2;

        if (1 == ischinese) {
            if (str1nowlen + 1 >= str1len) break;

            strncpy(str1 + str1nowlen, str + i, 1);
            len += 1;
            str1nowlen += 1;
        } else if (2 == ischinese) {
            if (str1nowlen + 3 >= str1len) break;
            strncpy(str1 + str1nowlen, str + i, 3);
            i += 2;
            len += 1.5;
            str1nowlen += 3;
        }
        
        // 加换行
        if ((i / 9 > sumflag) && (i % 9 == 0)) {
            if (str1nowlen + 1 >= str1len) break;
            strncpy(str1 + str1nowlen, "\n", 1);
            str1nowlen += 1;
        }

        if (clen <= len) break;
    }

    return;
}


int main (int argc, char* argv[])
{
    char str1[] = "中文123sdf_中文中文男男女女男男女女男男女女奶奶nnnnnnnnnnn";
    char buf[1000] = {0};
    int cutstr = 0;
    // 行的长度 默认是18,中文2个，英文1个

    cutstr = 18;
    string_length (str1, cutstr, buf, sizeof buf);
    printf ("%s\n %d\n%s\n", str1, cutstr, buf);

    cutstr = 36;
    string_length (str1, cutstr, buf, sizeof buf);
    printf ("%s\n %d\n%s\n", str1, cutstr, buf);

    return 0;
}
