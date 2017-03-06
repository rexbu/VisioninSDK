/**
 * file :	bs_data.h
 * author :	Rex
 * create :	2016-11-15 17:53
 * func : 
 * history:
 */

#ifndef	__BS_DATA_H_
#define	__BS_DATA_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_object.h"

#ifdef __cplusplus
extern "C"{
#endif
    
#define STRING_DEF_SIZE		64
    
    typedef struct data_t{
        object_t	base;
        uint32_t	size;
        uint32_t	len;
        char*		mem;
        status_t (*set)(struct data_t* str, const byte* s, uint32_t size);
        status_t (*append)(struct data_t* str, const byte* s, uint32_t size);
    }data_t;
    
    void* data_init(void* p);
    void data_destroy(void* p);
    status_t data_set(struct data_t* str, const byte* s, uint32_t size);
    status_t data_append(data_t* str, const byte* s, uint32_t size);
    
#define data_memory(s)      ((s)->mem)
    // 内容长度，data的长度用size，string的长度用length
#define data_size(s)       	((s)->len)
    
#ifdef __cplusplus
}
#endif
#endif
