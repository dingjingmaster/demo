/*************************************************************************
> FileName: pam_test_mypasswd1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月08日 星期三 21时34分25秒
 ************************************************************************/
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
/*
  This program was contributed by Shane Watts
  [modifications by AGM and kukuk]

  You need to add the following (or equivalent) to the
  /etc/pam.d/check_user file:
  # check authorization
  auth       required     pam_unix.so
  account    required     pam_unix.so
 */

static struct pam_conv conv = {
    misc_conv,
    NULL
};

/*
struct pam_message
{
    int msg_style;
    const char *msg;
};

struct pam_response
{
    char *resp;
    int resp_retcode;
};

struct pam_conv
{
    int (*conv)(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);
    void *appdata_ptr;
};

*/
int main(int argc, char *argv[])
{
    pam_handle_t *pamh=NULL;
    int retval;
    const char *user="dingjing";

    printf ("1\n");
    if(argc == 2) {
        user = argv[1];
    }
    printf ("2\n");

    if(argc > 2) {
        printf ("使用: xxx username\n");
        exit(1);
    }

    printf ("begin!\n");
    retval = pam_start("check_user", user, &conv, &pamh);
    printf ("check_user!\n");

    if (retval == PAM_SUCCESS) {
        retval = pam_authenticate(pamh, 0);    /* is user really user? */
        printf ("really user!\n");
    }

    if (retval == PAM_SUCCESS) {
        retval = pam_acct_mgmt(pamh, 0);       /* permitted access? */
        printf ("can permitted access!\n");
    }

    /* This is where we have been authorized or not. */
    if (retval == PAM_SUCCESS) {
        printf ("Authenticated!\n");
    } else {
        printf ("Not Authenticated!\n");
    }

    if (pam_end(pamh,retval) != PAM_SUCCESS) {
        pamh = NULL;
        printf ("failed to release authenticator!\n");
        exit(1);
    }

    return ( retval == PAM_SUCCESS ? 0:1 );
}
