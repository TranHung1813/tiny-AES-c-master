#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
// #define CBC 1
// #define CTR 1
#define ECB 1

#include "aes.h"
#include "base64.h"

static void phex(uint8_t* str);
static void DecodeData(uint8_t* Buffer, uint16_t buffLength, uint8_t* output);

static uint8_t Input_Buffer[1024];

int main(void)
{
    printf("\nplain text:\n");
    gets((char *)Input_Buffer);
    static char decoded_buffer[150];
    DecodeData(Input_Buffer, 150, (uint8_t *)decoded_buffer);
    printf("\nciphertext:\n");
    printf("%s\r\n\n", decoded_buffer);

    return 0;
}

static void DecodeData(uint8_t* Buffer, uint16_t buffLength, uint8_t* output)
{
    static uint8_t MessageBufferBase64[256];
	uint16_t AESMessageLength = 0;
	// Check checksum
	memset(MessageBufferBase64, 0, 256);
	
	AESMessageLength = b64_decode(&Buffer[0], buffLength, MessageBufferBase64);

    for (uint8_t i = 0; i < 5; ++i)
    {
      phex(MessageBufferBase64 + (i * 16));
    }
    printf("\n");
	AES_ECB_decrypt(MessageBufferBase64, (uint8_t*)PUBLIC_KEY, output, AESMessageLength);
}

// prints string as hex
static void phex(uint8_t* str)
{

#if defined(AES128)
    uint8_t len = 16;
#endif

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x ", str[i]);
    printf("\n");
}
