#include <stdio.h>
#include <pwquality.h>
#include <string.h>

#define PWCONF "/etc/security/pwquality.conf"
#define RFLAG 0x1
#define CFLAG 0x2

int main(int argc, char *argv[])
{
    int ret, minlen;
    pwquality_settings_t *settings;
    void *auxerror;
    char buf[256], passwd[256];
    char flag;

    memset(&flag, 0, sizeof(char));
    switch (argc) {
        case 2:
            if (strncmp(argv[1], "read", 4)) {
                fprintf(stderr, "%s read\n", argv[0]);
                return -1;
            }
            flag |= RFLAG;
            break;
        case 3:
            if (strncmp(argv[1], "check", 5)) {
                fprintf(stderr, "%s check <password>\n", argv[0]);
                return -1;
            }
            flag |= CFLAG;
            strncpy(passwd, argv[2], 256);
            break;
        default:
            fprintf(stderr, "%s\n", "Parameter error");
            fprintf(stderr, "%s\tread\n\tcheck <password>\n", argv[0]);
            return -1;
    }

    settings = pwquality_default_settings();
    if (settings == NULL) {
        fprintf(stderr, "%s\n", "init pwquality settings failed");
        return -1;
    }

    ret = pwquality_read_config(settings, PWCONF, &auxerror);
    if (ret != 0) {
        fprintf(stderr, "Reading pwquality configuration file failed: %s\n", 
                pwquality_strerror(buf, sizeof(buf), ret, auxerror));
        goto out;
    }

    ret = pwquality_get_int_value(settings, PWQ_SETTING_MIN_LENGTH, &minlen);
    if (ret) {
        fprintf(stderr, "Getting minlen failed: %s\n",
                pwquality_strerror(buf, sizeof(buf), ret, auxerror));
        goto out;
    }

    if (flag & RFLAG) {
        printf("pwquality settings:\n");
        printf("menlen=%d\n", minlen);
        goto out;
    }

    if (flag & CFLAG) {
        ret = pwquality_check(settings, passwd, NULL, NULL, &auxerror);
        if (ret < 0) {
            fprintf(stderr, "Password too weak: %s\n",
                pwquality_strerror(buf, sizeof(buf), ret, auxerror));
            goto out;
        } else {
            printf("Password OK, score:%d\n", ret);
        }
    }

out:
    pwquality_free_settings(settings);
    return ret;
}
