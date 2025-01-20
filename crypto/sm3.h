#ifndef SM3_H_
#define SM3_H_
#include <stdint.h>
#define SM3_DIGEST_SIZE         32
#pragma pack(push, 1)
typedef struct sm3_context
{
  uint32_t total_bytes_High;
  uint32_t total_bytes_Low;
  uint32_t vector[8];
  uint8_t  buffer[64];
}sm3_context;
#pragma pack(pop)

int sm3_init(sm3_context *ctx);

int sm3_update(sm3_context *ctx, const unsigned char *chunk_data, unsigned int chunk_length);

int sm3_finish(sm3_context *ctx, unsigned char output[SM3_DIGEST_SIZE]);

int sm3_hash(const unsigned char *chunk_data, unsigned int chunk_length,
             unsigned char output[SM3_DIGEST_SIZE]);
#endif /* SM3_H_ */
