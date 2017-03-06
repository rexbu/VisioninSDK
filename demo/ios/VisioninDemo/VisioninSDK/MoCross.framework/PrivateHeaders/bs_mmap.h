/**
 * bushaofeng. 2011.8.17
 * bs_mmap.c
 * history:
 */

#ifndef BS_MMAP_H_
#define BS_MMAP_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_def.h"

#ifdef __cplusplus
extern "C"{
#endif

void* bs_mmap_create(int size);
void* bs_mmap_file(char* file, size_t size);

#ifdef __cplusplus
}
#endif

#endif
