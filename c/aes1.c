/*************************************************************************
> FileName: aes1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年12月23日 星期三 20时13分44秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
/*#include <openssl/rand.h>*/

// a simple hex-print routine. could be modified to print 16 bytes-per-line
static void hex_print(const void* pv, size_t len)
{
    const unsigned char * p = (const unsigned char*)pv;
    if (NULL == pv) {
        printf("NULL");
    } else {
        size_t i = 0;
        for (; i<len;++i) {
            printf("%02X ", *p++);
        }
    }
    printf("\n");
}

// main entrypoint
int main(int argc, char **argv)
{
    int keylength = 128;
    /* generate a key with a given length */
    unsigned char aes_key[16] = "123456789";

    size_t inputslength = 0;

    /* generate input with a given length */
    unsigned char aes_input[32] = "hello world";
    inputslength = strlen(aes_input) + 1;

    /* init vector */
    unsigned char iv_enc[AES_BLOCK_SIZE], iv_dec[AES_BLOCK_SIZE];

    /*RAND_bytes(iv_enc, AES_BLOCK_SIZE);*/
    memset(iv_enc, 0x00, AES_BLOCK_SIZE);
    memcpy(iv_dec, iv_enc, AES_BLOCK_SIZE);

    // buffers for encryption and decryption
    const size_t encslength = ((inputslength + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char enc_out[encslength];
    unsigned char dec_out[inputslength];
    memset(enc_out, 0, sizeof(enc_out));
    memset(dec_out, 0, sizeof(dec_out));

    // so i can do with this aes-cbc-128 aes-cbc-192 aes-cbc-256


    AES_KEY enc_key, dec_key;
    AES_set_encrypt_key(aes_key, keylength, &enc_key);
    AES_cbc_encrypt(aes_input, enc_out, inputslength, &enc_key, iv_enc, AES_ENCRYPT);

    AES_set_decrypt_key(aes_key, keylength, &dec_key);
    AES_cbc_encrypt(enc_out, dec_out, encslength, &dec_key, iv_dec, AES_DECRYPT);

    printf("original:\t");
    hex_print(aes_input, sizeof(aes_input));

    printf("encrypt:\t");
    hex_print(enc_out, sizeof(enc_out));

    printf("decrypt:\t");
    hex_print(dec_out, sizeof(dec_out));

    return 0;
}
