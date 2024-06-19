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
static void test_encrypt_ecb_verbose(void);

static uint8_t Input_Buffer[1024];
static uint8_t AES_output_Buffer[1024];
static uint8_t Base64_output_Buffer[1024];

int main(void)
{
    test_encrypt_ecb_verbose();


    return 0;
}

static void test_encrypt_ecb_verbose(void)
{
    uint8_t i;

    // print text to encrypt, key and IV
    printf("\n\n");
    printf("plain text:\n");
    //fgets((char *)Input_Buffer, sizeof(Input_Buffer), stdin );

    gets((char *)Input_Buffer);

    printf("%lld\r\n", strlen((char *)Input_Buffer));
    printf("\n");

    printf("ciphertext:\n");

    AES_ECB_encrypt(Input_Buffer, (uint8_t*)PUBLIC_KEY, AES_output_Buffer, strlen((char *)Input_Buffer));
    b64_encode(AES_output_Buffer, strlen((char *)Input_Buffer), (char *)Base64_output_Buffer);
    for (i = 0; i < 5; ++i)
    {
      phex(AES_output_Buffer + (i * 16));
    }
    printf("\n");

    // for (i = 0; i < 5; ++i)
    // {
    //   phex(Base64_output_Buffer + (i * 16));
    // }
    printf("%s\r\n\n", Base64_output_Buffer);
    printf("\n");
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
