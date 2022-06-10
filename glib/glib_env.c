/*************************************************************************
> FileName: glib_env.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时32分50秒
 ************************************************************************/

#include <glib.h>

int main(int argc, char *argv[]) 
{
    printf("Host name\t: %s\n", g_get_host_name());

    // 取得使用者真實姓名
    printf("Real name\t: %s\n", g_get_real_name());

    // 取得使用者帳號名稱
    printf("User name\t: %s\n", g_get_user_name());

    // 取得目前所在路徑
    printf("Current dir\t: %s\n", g_get_current_dir());

    // 取得使用者家目錄
    printf("Home dir\t: %s\n", g_get_home_dir());

    // 取得暫存目錄
    printf("Temp dir\t: %s\n", g_get_tmp_dir());

    // 取得PATH環境變數
    printf("PATH\t\t: %s\n", g_getenv("PATH"));

    // 設定CLASSPATH環境變數，FALSE表示若已設定則不覆寫
    g_setenv("CLASSPATH", "D:\\Temp", FALSE);

    // 取得CLASSPATH環境變數
    printf("CLASSPATH\t: %s\n", g_getenv("CLASSPATH"));

    // 取消CLASSPATH環境變數
    g_unsetenv("CLASSPATH");

    printf("CLASSPATH\t: %s\n", g_getenv("CLASSPATH"));

    return 0;
}
