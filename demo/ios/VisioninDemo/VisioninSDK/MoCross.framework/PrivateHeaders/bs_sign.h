/**
 * bushaofeng. 2012.3.17
 */

#ifndef __BS_SIGN_H_
#define __BS_SIGN_H_

#include "bs_type.h"
#include "bs_error.h"

#ifdef __cplusplus
extern "C"{
#endif

#define BS_MD5_STRLEN           32 

/*
typedef struct{
    uint16_t            sign1;
    unsigned char       sign2[BS_MD5_STRLEN-sizeof(uint16_t)];
}bs_sign_t;
*/
typedef char            bs_sign_t[BS_MD5_STRLEN+1];

uint32_t bs_crc32(uint32_t crc, void* buf, uint32_t size);
int bs_sign(char* buf, bs_sign_t sign);
uint64_t bs_usign(char* buf);

#ifdef __cplusplus
}
#endif

#endif
