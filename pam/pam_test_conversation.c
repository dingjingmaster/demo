/*************************************************************************
> FileName: pam_test_conversation.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月09日 星期四 22时35分52秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <syslog.h>

#define CT_SYSLOG(logLevel,...) syslog(logLevel, __FILE__, __func__, __LINE__, ##__VA_ARGS__);

static struct pam_conv conv = {
    misc_conv,
    //my_conv,
    NULL
};

int main(int argc, char *argv[])
{
    CT_SYSLOG (LOG_ERR, "");

    pam_handle_t *pamh=NULL;
    int retval;
    const char *user="dingjing";

    if (argc == 2) {
        user = argv[1];
    } else if (argc > 2) {
        exit(1);
    }

    CT_SYSLOG (LOG_ERR, "开始PAM事务");
    retval = pam_start("test", user, &conv, &pamh);
    if (retval == PAM_SUCCESS) {
        retval = pam_authenticate(pamh, 0);
    }

    //retval = pam_set_data (pamh, "test", "demo", &cleanup_3);
    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        exit(1);
    }

    CT_SYSLOG (LOG_ERR, "验证结束");

    return (retval == PAM_SUCCESS?0:1);
}

