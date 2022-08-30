/*************************************************************************
> FileName: string.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 20 Aug 2022 04:57:44 PM CST
 ************************************************************************/
#include <string>

#include <stdio.h>

using namespace std;

string split_str_kv_at (string& str)
{
    int pos1 = 0;

    int pos1S = pos1;
    pos1 = str.find ('@', pos1S);
    if (pos1 == -1) {
        return "";
    } else {
        return str.substr (pos1S, pos1 - pos1S);
    }
}



void split_str (string& str)
{
    int pos1 = 0;

    for (;pos1 != -1;) {
        int pos1S = pos1;
        pos1 = str.find ('&', pos1S);
        //printf ("%d\n", pos1);
        if (pos1 == -1) {
            /* 处理 key 的逻辑 */
            string kvStr = str.substr (pos1S, str.size() - pos1S);
            string key = split_str_kv_at (kvStr);
            if ("" != key) {
                printf ("%s -- %s\n", (char*)str.c_str(), (char*)key.c_str());
                // 处理 key 的逻辑
            }
            break;
        } else {
            string kvStr = str.substr (pos1S, pos1 - pos1S);
            string key = split_str_kv_at (kvStr);
            if ("" != key) {
                printf ("%s -- %s\n", (char*)str.c_str(), (char*)key.c_str());
                // 处理 key 的逻辑
            }
            ++pos1;
        }
    }
}

int main (int argc, char* argv[])
{
    string str1 = "aaaaaa@111111";
    string str2 = "aaaaaa@111111&bbbbbb@222222";
    string str3 = "aaaaaa@111111&bbbbbb@222222&eweweq@12313";

    split_str (str1);
    printf ("=====\n");
    split_str (str2);
    printf ("=====\n");
    split_str (str3);

    return 0;
}
