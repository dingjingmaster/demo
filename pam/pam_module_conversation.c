/*************************************************************************
> FileName: pam_module_conversation.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月09日 星期四 22时54分56秒
 ************************************************************************/
#include <sys/param.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>

#include <security/pam_modules.h>
#include <security/pam_appl.h>

#define CT_SYSLOG(logLevel,...) syslog(logLevel, __FILE__, __func__, __LINE__, ##__VA_ARGS__);

/**
 * 1. 将文件复制到 /lib64/secure/
 * 2. 配置/etc/pam.d/test
 * 3. 新加如下条：
 *      auth       required     pam_module_conversation.so
 *      password   required     pam_module_conversation.so
 *      account    required     pam_module_conversation.so
 *      session    required     pam_module_conversation.so
 */

// 密码验证
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,int argc, const char *argv[])
{
    syslog_init("PAM服务端", LOG_LOCAL6, 1);

    struct passwd *pwd;
    const char *user;
    char *crypt_password, *password;
    int pam_err, retry;

    pam_set_item(pamh, PAM_USER_PROMPT, "用户名: ");

    // identify user
    if ((pam_err = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
        return (pam_err);

    if ((pwd = getpwnam(user)) == NULL)
        return (PAM_USER_UNKNOWN);
    // 没法输出
    //CT_SYSLOG (LOG_ERR, "user name:%s, true password:%s", user, pwd);

    // get password 
    for (retry = 0; retry < 3; ++retry) {
         pam_err = pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
        if (pam_err == PAM_SUCCESS)
            break;
    }

    
    pam_prompt (pamh, PAM_PROMPT_ECHO_OFF, NULL, "XXX%s^^^^", "测试提示!");

    // 没法输出
    CT_SYSLOG (LOG_ERR, "user name:%s, true password:%s, input password:%s", (char*)user, pwd->pw_passwd, (char*)password);

    CT_SYSLOG (LOG_ERR, "============= 服务端信息 =============");
    char** var = pam_getenvlist(pamh);
    if (*var != NULL) {
        int i = 0;
        for (i = 0; var[i] != NULL; ++i) {
            CT_SYSLOG (LOG_ERR, "%s", var[i])
        }
    }
    CT_SYSLOG (LOG_ERR, "======================================");

    if (pam_err == PAM_CONV_ERR)
        return (pam_err);

    if (pam_err != PAM_SUCCESS)
        return (PAM_AUTH_ERR);

    // 比较密码是否相同

    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
    syslog_init("PAM服务端", LOG_LOCAL6, 1);
    CT_SYSLOG (LOG_ERR, "");

    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
    syslog_init("PAM服务端", LOG_LOCAL6, 1);
    CT_SYSLOG (LOG_ERR, "");

    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
    syslog_init("PAM服务端", LOG_LOCAL6, 1);
    CT_SYSLOG (LOG_ERR, "");

    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
    syslog_init("PAM服务端", LOG_LOCAL6, 1);
    CT_SYSLOG (LOG_ERR, "");

    return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{
    syslog_init("PAM服务端", LOG_LOCAL6, 1);
    CT_SYSLOG (LOG_ERR, "");

    return (PAM_SERVICE_ERR);
}
