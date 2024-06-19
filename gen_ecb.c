#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
// #define CBC 1
// #define CTR 1
#define ECB 1

#include "aes.h"
#include "base64.h"

static void phex(uint8_t* str);
static void encrypt_ecb(uint8_t* Buffer, uint16_t buffLength, Padding_t padding, uint8_t* output);

static uint8_t Input_Buffer[1024];
static uint8_t AES_output_Buffer[1024];
static uint8_t Base64_output_Buffer[1024];

int main(int argc, char **argv)
{
    printf("\n");
    if (argc == 1)
    {
        printf("Please use format: gen + [  padding  ]\r\n");
        printf("Example:           gen         0             \r\n\n");
        printf("-------------------------------------------\r\n");
        printf("[ Keysize: 128 bits                        ]\r\n");
        printf("[ Ciphermode: ECB                          ]\r\n");
        printf("[ Padding: 0 - NoPadding, 1 - PKCS7Padding ]\r\n");
        printf("-------------------------------------------\r\n");
        return 0;
    }
    else if (argc == 2)
    {
        if (strstr(argv[1], "help"))
        {
            printf("Please use format: gen + [  padding  ]\r\n");
            printf("Example:           gen         0             \r\n\n");
            printf("-------------------------------------------\r\n");
            printf("[ Keysize: 128 bits                        ]\r\n");
            printf("[ Ciphermode: ECB                          ]\r\n");
            printf("[ Padding: 0 - NoPadding, 1 - PKCS7Padding ]\r\n");
            printf("-------------------------------------------\r\n");
            return 0;
        }
        else
        {
            int padding = atoi(argv[1]);
            if (padding != NoPadding && padding != PKCS7Padding) return 0;
            printf("plain text:\n");
            //fgets((char *)Input_Buffer, sizeof(Input_Buffer), stdin );

            gets((char *)Input_Buffer);

            printf("length: %lld\r\n", strlen((char *)Input_Buffer));
            printf("\n");

            printf("ciphertext:\n");

            encrypt_ecb(Input_Buffer, strlen((char *)Input_Buffer), padding, Base64_output_Buffer);

            printf("\n");

            // for (i = 0; i < 5; ++i)
            // {
            //   phex(Base64_output_Buffer + (i * 16));
            // }
            printf("%s\r\n\n", Base64_output_Buffer);
            printf("\n");
        }
    }
    else
    {
        printf("Invalid %u arguments!\r\n", argc);
        printf("Please use format: gen + [  padding  ]\r\n");
        printf("Example:           gen         0             \r\n\n");
        return 0;
    }

    return 0;
}

static void encrypt_ecb(uint8_t* Buffer, uint16_t buffLength, Padding_t padding, uint8_t* output)
{
    uint8_t i;

    AES_ECB_encrypt(Buffer, (uint8_t*)PUBLIC_KEY, padding, AES_output_Buffer, buffLength);
    b64_encode(AES_output_Buffer, strlen((char *)Buffer), (char *)output);
    for (i = 0; i < 5; ++i)
    {
      phex(AES_output_Buffer + (i * 16));
    }
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
