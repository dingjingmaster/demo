#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>


int main (int argc, char* argv[])
{
    if (argc < 3) {
        printf("请输入要检查的设备和挂载点\n./%s <dev> <mount point>\n", argv[0]);
        return -1;
    }

    const char* dev = argv[1];
    const char* mountPoint = argv[2];

    printf ("dev        : %s\n", dev);
    printf ("mount point: %s\n", mountPoint);
    printf ("\n");

#define CHECK_MTAB "/etc/mtab"
    if (0 == access(CHECK_MTAB, F_OK)) {
        printf ("Start check by '%s'\n", CHECK_MTAB);
        char c = '\0';
        FILE* fr = fopen (CHECK_MTAB, "r");
        if (fr) {
            uint32_t lineIdx = 0;
            uint32_t bufLen = 32;
            char* lineBuf = (char*) malloc(bufLen);
            if (NULL == lineBuf) {
                printf("malloc error\n");
                goto out;
            }
            memset(lineBuf, 0, bufLen);

            while ((c = fgetc(fr)) != EOF) {
                if (c != '\n') {
                    if (lineIdx >= bufLen - 1) {
                        uint32_t bufLenTmp = 2 * bufLen;
                        char* bufTmp = (char*) malloc (bufLenTmp);
                        if (NULL == bufTmp) {
                            goto out;
                        }
                        memset(bufTmp, 0, bufLenTmp);
                        memcpy(bufTmp, lineBuf, bufLen);
                        free (lineBuf);
                        lineBuf = bufTmp;
                        bufLen = bufLenTmp;
                    }

                    lineBuf[lineIdx] = c;
                    ++lineIdx;
                    continue;
                }

                // read a line
                if (lineBuf[lineIdx] == '\r') {
                    lineBuf[lineIdx] = '\0';
                }

                if (lineBuf == strstr(lineBuf, dev)) {
                    if (NULL != strstr(lineBuf, mountPoint)) {
                        printf("Is mounted. line: '%s'\n", lineBuf);
                        break;
                    }
                }

                lineIdx = 0;
                memset(lineBuf, 0, bufLen);
            }
out:
            if (lineBuf) {
                free (lineBuf);
            }

            if (fr) {
                fclose(fr);
            }
        }
    }

#define CHECK_PROC "/proc/mounts"
    printf("Check '%s'\n", CHECK_PROC);


    return 0;
}
