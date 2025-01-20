#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sm3.h"

#define rol(x,n) (((uint32_t)x << (n & 0x1F)) | ((uint32_t)(x & 0xFFFFFFFF) >> ((32 - n) & 0x1F)))
#define P0(x) ((x^(rol(x,9))^(rol(x,17))))
#define P1(x) ((x^(rol(x,15))^(rol(x,23))))

#define CONCAT_4_BYTES( w32, w8, w8_i)            \
{                                                 \
    (w32) = ( (uint32_t) (w8)[(w8_i)    ] << 24 ) |  \
            ( (uint32_t) (w8)[(w8_i) + 1] << 16 ) |  \
            ( (uint32_t) (w8)[(w8_i) + 2] <<  8 ) |  \
            ( (uint32_t) (w8)[(w8_i) + 3]       );   \
}

#define SPLIT_INTO_4_BYTES( w32, w8, w8_i)        \
{                                                 \
    (w8)[(w8_i)] = (uint8_t) ( (w32) >> 24 );    \
    (w8)[(w8_i) + 1] = (uint8_t) ( (w32) >> 16 );    \
    (w8)[(w8_i) + 2] = (uint8_t) ( (w32) >>  8 );    \
    (w8)[(w8_i) + 3] = (uint8_t) ( (w32)       );    \
}


//#define _GNU_SOURCE

#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

static uint8_t SM3_padding[64] =
{
	(uint8_t) 0x80, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0,
	(uint8_t)    0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0,
	(uint8_t)    0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0,
	(uint8_t)    0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0
};

static void SM3_CF(sm3_context *ctx, const unsigned char *byte_64_block )
{
	uint32_t j,temp,W[68];

	uint32_t A,B,C,D,E,F,G,H,SS1,SS2,TT1,TT2;
#ifdef DEBUG_PRINT
	for(j=0;j<64;j++)
	printf("%d:%8x\n",j,byte_64_block[j]);
	printf("\n--------------------\n");
#endif

	CONCAT_4_BYTES( W[0],  byte_64_block,  0 );
	CONCAT_4_BYTES( W[1],  byte_64_block,  4 );
	CONCAT_4_BYTES( W[2],  byte_64_block,  8 );
	CONCAT_4_BYTES( W[3],  byte_64_block, 12 );
	CONCAT_4_BYTES( W[4],  byte_64_block, 16 );
	CONCAT_4_BYTES( W[5],  byte_64_block, 20 );
	CONCAT_4_BYTES( W[6],  byte_64_block, 24 );
	CONCAT_4_BYTES( W[7],  byte_64_block, 28 );
	CONCAT_4_BYTES( W[8],  byte_64_block, 32 );
	CONCAT_4_BYTES( W[9],  byte_64_block, 36 );
	CONCAT_4_BYTES( W[10], byte_64_block, 40 );
	CONCAT_4_BYTES( W[11], byte_64_block, 44 );
	CONCAT_4_BYTES( W[12], byte_64_block, 48 );
	CONCAT_4_BYTES( W[13], byte_64_block, 52 );
	CONCAT_4_BYTES( W[14], byte_64_block, 56 );
	CONCAT_4_BYTES( W[15], byte_64_block, 60 );
#ifdef DEBUG_PRINT
	for(j=0;j<16;j++)
	{
		printf("%d:%8x\n",j,W[j]);
	}
#endif
	for(j=16;j<68;j++)
	{
		//waitting to modified 
		//there is something strange here,"P1(W[j-16]^W[j-9]^rol(W[j-3],15))" will get a error result
		temp = W[j-16]^W[j-9]^rol(W[j-3],15);
		W[j] = P1(temp)^rol(W[j-13],7)^(W[j-6]);
		//W[j] = P1((W[j-16]^W[j-9]^rol(W[j-3],15)))^rol(W[j-13],7)^(W[j-6]);

#ifdef DEBUG_PRINT

		printf("%d::(*)=%8x :temp=%8x :P1((*))=%8x :P1(*)=%8x :p1(temp)=%8x\n",
			j,(W[j-16]^W[j-9]^rol(W[j-3],15)),P1((W[j-16]^W[j-9]^rol(W[j-3],15))),temp,P1(W[j-16]^W[j-9]^rol(W[j-3],15)),P1(temp));

#endif
	}
	A = ctx->vector[0];
	B = ctx->vector[1];
	C = ctx->vector[2];
	D = ctx->vector[3];
	E = ctx->vector[4];
	F = ctx->vector[5];
	G = ctx->vector[6];
	H = ctx->vector[7];
#ifdef DEBUG_PRINT
	printf(" :A:%08x,B:%08x,C:%08x,D:%08x,E:%08x,F:%08x,G:%08x,H:%08x\n",A,B,C,D,E,F,G,H);
#endif

#define T 0x79cc4519
#define FF(X,Y,Z) (X^Y^Z)
#define GG(X,Y,Z) (X^Y^Z)

	for(j=0;j<16;j++)
	{
		SS1 = rol((rol(A,12) + E + rol(T,j)),7);
		SS2 = SS1^(rol(A,12));
		TT1 = FF(A,B,C) + D + SS2 + (W[j]^W[j+4]);
		TT2 = GG(E,F,G) + H + SS1 + W[j];
		D = C;
		C = rol(B,9);
		B = A;
		A = TT1;
		H = G;
		G = rol(F,19);
		F = E;
		E = P0(TT2);
#ifdef DEBUG_PRINT
		printf("%d: A:%08x,B:%08x,C:%08x,D:%08x,E:%08x,F:%08x,G:%08x,H:%08x\n",j,A,B,C,D,E,F,G,H);
#endif
	}

#undef T
#undef FF 
#undef GG

#define T 0x7a879d8a 
#define FF(X,Y,Z) ((X&Y)|(X&Z)|(Y&Z))
#define GG(X,Y,Z) ((X&Y)|(~X&Z))

	for(j=16;j<64;j++)
	{
		SS1 = rol((rol(A,12) + E + rol(T,j)),7);
		SS2 = SS1^(rol(A,12));
		TT1 = FF(A,B,C) + D + SS2 + (W[j]^W[j+4]);
		TT2 = GG(E,F,G) + H + SS1 + W[j];
		D = C;
		C = rol(B,9);
		B = A;
		A = TT1;
		H = G;
		G = rol(F,19);
		F = E;
		E = P0(TT2);
#ifdef DEBUG_PRINT
		printf("%d: A:%08x,B:%08x,C:%08x,D:%08x,E:%08x,F:%08x,G:%08x,H:%08x\n",j,A,B,C,D,E,F,G,H);
#endif
	}
	
#undef T
#undef FF 
#undef GG

	ctx->vector[0] ^= A;
	ctx->vector[1] ^= B;
	ctx->vector[2] ^= C;
	ctx->vector[3] ^= D;
	ctx->vector[4] ^= E;
	ctx->vector[5] ^= F;
	ctx->vector[6] ^= G;
	ctx->vector[7] ^= H;
}
int sm3_init(sm3_context *ctx){
	ctx->total_bytes_High = 0;
	ctx->total_bytes_Low = 0;
	ctx->vector[0] = 0x7380166f;
	ctx->vector[1] = 0x4914b2b9;
	ctx->vector[2] = 0x172442d7;
	ctx->vector[3] = 0xda8a0600;
	ctx->vector[4] = 0xa96f30bc;
	ctx->vector[5] = 0x163138aa;
	ctx->vector[6] = 0xe38dee4d;
	ctx->vector[7] = 0xb0fb0e4e;
	return 0;
}
int sm3_update(sm3_context *ctx, const unsigned char *chunk_data, unsigned int chunk_length)
{
	int left, fill;
	int i;
	left = ctx->total_bytes_Low & 0x3F;
	fill = 64 - left;
	ctx->total_bytes_Low += chunk_length;
	ctx->total_bytes_Low &= 0xFFFFFFFF;

	if (ctx->total_bytes_Low < (unsigned int)chunk_length)
	{
		ctx->total_bytes_High++;
	}

	if ((left > 0) && (chunk_length >= fill))
	{
		for ( i = 0; i < fill; i++ )
		{
			ctx->buffer[left + i] = chunk_data[i];
		}
		SM3_CF( ctx, ctx->buffer );
		chunk_length -= fill;
		chunk_data  += fill;
		left = 0;
	}

	while( chunk_length >= 64 )
	{
		SM3_CF( ctx, chunk_data );
		chunk_length -= 64;
		chunk_data  += 64;
	}

	if ( chunk_length > 0 )
	{
		for ( i = 0; i < chunk_length; i++ )
		{
			ctx->buffer[left + i] = chunk_data[i];
		}
	}
	return 0;
}

int sm3_finish (sm3_context *ctx, unsigned char output[SM3_DIGEST_SIZE])
{
  uint32_t last, padn;
  uint32_t high, low;
  uint8_t  msglen[8];
  int   ret;

  high = ( ctx->total_bytes_Low >> 29 ) | ( ctx->total_bytes_High <<  3 );
  low  = ( ctx->total_bytes_Low <<  3 );
  SPLIT_INTO_4_BYTES( high, msglen, 0 );
  SPLIT_INTO_4_BYTES( low,  msglen, 4 );

  last = ctx->total_bytes_Low & 0x3F;
  padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );
  ret = sm3_update( ctx, SM3_padding, padn );
  if (ret != 0)
  	return ret;
  
  ret = sm3_update( ctx, msglen, 8 );
  if (ret != 0)
  	return ret;
  
  PUT_ULONG_BE( ctx->vector[0], output,  0 );
  PUT_ULONG_BE( ctx->vector[1], output,  4 );
  PUT_ULONG_BE( ctx->vector[2], output,  8 );
  PUT_ULONG_BE( ctx->vector[3], output, 12 );
  PUT_ULONG_BE( ctx->vector[4], output, 16 );
  PUT_ULONG_BE( ctx->vector[5], output, 20 );
  PUT_ULONG_BE( ctx->vector[6], output, 24 );
  PUT_ULONG_BE( ctx->vector[7], output, 28 );
  return 0;

}

int sm3_hash (const unsigned char *chunk_data, unsigned int chunk_length,
             unsigned char output[SM3_DIGEST_SIZE])
{
  sm3_context ctx;
  sm3_init( &ctx );
  sm3_update( &ctx, chunk_data, chunk_length );
  return sm3_finish( &ctx, output );
 }