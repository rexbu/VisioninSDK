/**
 * file :	bs_string.h
 * author :	bushaofeng
 * create :	2016-08-25 01:03
 * func : 
 * history:
 */

#ifndef	__BS_STRING_H_
#define	__BS_STRING_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_object.h"

#ifdef __cplusplus
extern "C"{
#endif

#define STRING_DEF_SIZE		64

typedef struct string_t{
    object_t	base;
	uint32_t	size;
	uint32_t	len;
	char*		mem;
    status_t (*set)(struct string_t* str, const char* s);
    status_t (*append)(struct string_t* str, const char* s);
}string_t;

void* string_init(void* p);
void string_destroy(void* p);
status_t string_set(struct string_t* str, const char* s);
status_t string_append(string_t* str, const char* s);

#define string_memory(s)    ((s)->mem)
// 内容长度，data的长度用size，string的长度用length
#define string_size(s)      ((s)->len)

#define string_char(s)      ((s)->mem)
#define string_len(s)       ((s)->len)
#define string_substr(s, i) ((s)->mem+i)
    
#ifdef __cplusplus
}
#endif
#endif
