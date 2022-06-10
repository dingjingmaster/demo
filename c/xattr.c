/*************************************************************************
> FileName: xattr.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月18日 星期五 12时04分17秒
 ************************************************************************/
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>

int main(int argc, char *argv[])
{
    ssize_t buflen, keylen, vallen;
    char *buf, *key, *val;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * Determine the length of the buffer needed.
     **/
    buflen = listxattr(argv[1], NULL, 0);
    if (buflen == -1) {
        perror("listxattr");
        exit(EXIT_FAILURE);
    }

    if (buflen == 0) {
        printf("%s has no attributes.\n", argv[1]);
        exit(EXIT_SUCCESS);
    }

    /*
     * Allocate the buffer.
     * */
    buf = malloc(buflen);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /*
     * Copy the list of attribute keys to the buffer.
     **/
    buflen = listxattr(argv[1], buf, buflen);
    if (buflen == -1) {
        perror("listxattr");
        exit(EXIT_FAILURE);
    }

    /*
     * Loop over the list of zero terminated strings with the
     * attribute keys. Use the remaining buffer length to determine
     * the end of the list.
     * */
    key = buf;
    while (buflen > 0) {
        /*
         * Output attribute key.
         * */
        printf("%s: ", key);

        /*
         * Determine length of the value.
         * */
        vallen = getxattr(argv[1], key, NULL, 0);
        if (vallen == -1)
            perror("getxattr");

        if (vallen > 0) {
            /*
             * Allocate value buffer.
             * One extra byte is needed to append 0x00.
             * */
            val = malloc(vallen + 1);
            if (val == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            /*
             * Copy value to buffer.
             * */
            vallen = getxattr(argv[1], key, val, vallen);
            if (vallen == -1) {
                perror("getxattr");
            } else {
                /*
                 * Output attribute value.
                 * */
                val[vallen] = 0;
                printf("%s", val);
            }
            free(val);
        } else if (vallen == 0)
            printf("<no value>");
        printf("\n");

        /*
         * Forward to next attribute key.
         * */
        keylen = strlen(key) + 1;
        buflen -= keylen;
        key += keylen;
    }
    free(buf);
    exit(EXIT_SUCCESS);
}
