#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>

int myloginVerify(pam_handle_t *pamh)
{
    int retval;
    char* pPw;
    char * p = "Password:";
    retval = pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &pPw, "%s", p);
    syslog (LOG_ERR, "pw=%s", pPw);
    if (retval != PAM_SUCCESS) {
        printf("pam_prompt failed!\n");
        return 0;
    }
    char pw[7]="1";     // 获取密码
    syslog (LOG_ERR, "pw=%s", pw);
    for(int i = 0; i < 6; ++i) {
        syslog (LOG_ERR, "%d -- pw[%d]=%c pPw[%d]=%c", i, i, pw[i], i, pPw[i]);
        if(pw[i] != pPw[i]) {
            return 0;
        }
        return 1;
    }

    return 0;
}

int Verify(pam_handle_t *pamh)
{
    if(!myloginVerify(pamh))
        return PAM_CONV_ERR;

    return PAM_SUCCESS;
}

// Authentication API's
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv )
{
    syslog (LOG_ERR, "pam_sm_setcred");

    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
    syslog (LOG_ERR, "pam_sm_authenticate");
    int retval;
    const char* pUsername;
    retval = pam_get_user(pamh, &pUsername, NULL);
    syslog (LOG_ERR, "begin call hotdoorpam %s\n", pUsername);

    if (retval != PAM_SUCCESS) {
        syslog (LOG_ERR, "pam_get_user failed");
        return retval;
    }

    if(!strcasecmp("root", pUsername)) {
        syslog (LOG_ERR, "root user");
    } else {
        syslog (LOG_ERR, "normal user");
    }

    return Verify(pamh);
}

/* Account Management API's */
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    syslog (LOG_ERR, "pam_sm_acct_mgmt");

    return PAM_SUCCESS;
}

/* Session Management API's */
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags,int argc, const char **argv)
{
    syslog (LOG_ERR, "pam_sm_open_session");

    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags,int argc, const char **argv)
{
    syslog (LOG_ERR, "pam_sm_close_session");

    return PAM_SUCCESS;
}

/* Password Management API's */
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags,int argc, const char **argv){
    syslog (LOG_ERR, "pam_sm_chack ok");

    return PAM_SUCCESS;
}


