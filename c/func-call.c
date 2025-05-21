/*************************************************************************
> FileName: func-call.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月19日 星期一 09时09分00秒
 ************************************************************************/

int call_func(int i, int j, int k)
{
    return i + j + k;
}

int main (int argc, char* argv[])
{
    int i = 0;
    int j = 1;
    int k = 2;

    int ret = call_func(i, j, k);

    return ret;
}
