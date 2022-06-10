/*************************************************************************
> FileName: aes.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年12月23日 星期三 20时01分16秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#define DSP_DES_CBC_KEY     "123456789"

int main (int argc, char* argv[])
{
    unsigned char aes_key[AES_BLOCK_SIZE] = {0};
    unsigned char ivc[AES_BLOCK_SIZE] = {0};
    unsigned char enc_out[256] = {0};
    unsigned char tmp[128] = {0};

    int ret  = snprintf((char *)tmp, sizeof(tmp), "%s", "hello world");

    int encslength = ((ret + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    memset(ivc, 0, sizeof(ivc));
    AES_KEY enc_key;
    memcpy(aes_key, DSP_DES_CBC_KEY, sizeof(DSP_DES_CBC_KEY));
    AES_set_encrypt_key(aes_key, 128, &enc_key);
    AES_cbc_encrypt((unsigned char*)tmp, enc_out, encslength, &enc_key, ivc, AES_ENCRYPT);

    printf("AES_ENCRYPT: %s\n", enc_out);

    memset(ivc, 0, sizeof(ivc));
    unsigned char decrypt_string[256] = {0};
    AES_set_decrypt_key(aes_key, 128, &enc_key);
    AES_cbc_encrypt(enc_out, decrypt_string, encslength, &enc_key, ivc, AES_DECRYPT);

    printf("AES_DECRYPT: %s\n", decrypt_string);

    return 0;
}
