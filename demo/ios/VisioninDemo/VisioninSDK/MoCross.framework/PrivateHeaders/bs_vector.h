/**
 * file :	bs_vector.h
 * author :	bushaofeng
 * create :	2013-11-29 15:14
 * func : 
 * history:
 */

#ifndef	__BS_VECTOR_H_
#define	__BS_VECTOR_H_

#include "bs_type.h"
#include "bs_def.h"
#include "bs_error.h"
#include "bs_object.h"
 
#ifdef __cplusplus
extern "C"{
#endif

#define VECTOR_DEF_SIZE             64
#define VECTOR_REALLOC_SIZE         16

typedef struct _vector_t{
    object_t    base;
    uint32_t    size;
    uint32_t    len;
    void*       mem;
}_vector_t;

void* _vector_init(void* p);
void _vector_destroy(void* p);
int _vector_add(_vector_t* v, void* elem, uint32_t esize);
state_t _vector_find(_vector_t* v, void* elem, uint32_t esize);

#define vector_t(type)   struct{ \
    object_t    base;   \
    uint32_t    size;   \
    uint32_t    len;    \
    type*       mem;    \
    type        elem;   \
}

#define vector_init(v)          _vector_init(v);

#define vector_count(v)         ((v)->len)
#define vector_esize(v)         (sizeof((v)->elem))
#define vector_size(v)          ((v)->size)
#define vector_push(v, e)       _vector_add((_vector_t*)(v), &e, vector_esize(v))
#define vector_add(v, e)        vector_push(v, e)
#define vector_get(v, i)        ((i)>=vector_count(v) ? NULL:(&(v)->mem[i]))
#define vector_entity(v, i, d)  ((i)>=vector_count(v) ? (d):(v)->mem[i])
#define vector_set(v, e, i)     if((i)<vector_size(v)){    \
    (v)->mem[i] = e;        \
}

#define vector_remove(v, i)     if((i)<vector_count(v)){    \
    for (int j = i; j < vector_count(v)-1; ++j){        \
        (v)->mem[j] = (v)->mem[j+1];    \
    }\
    (v)->len--;\
}

#define vector_find(v, e)       _vector_find(v, &e, vector_esize(v))
#define vector_clear(v)         ((v)->len=0)
#define vector_destroy(v)       _vector_destroy(v)
// 设置vector重新申请容量时候的大小
#define vector_set_realloc_size(s)  do{     \
    extern uint32_t g_vector_realloc_size;  \
    g_vector_realloc_size = (s);            \
}while(0)
    
//typedef struct _vector_lock_t{
//    uint8_t             islock:1;
//    uint32_t            esize:31;
//    uint32_t            size;
//    uint32_t            vlen;
//    void*               mem;
//    pthread_rwlock_t    lock;
//}_vector_lock_t;
//
//#define vector_lock_t(type)  struct{    \
//    uint8_t             islock:1;  \
//    uint32_t            esize:31;  \
//    uint32_t            size;      \
//    uint32_t            vlen;      \
//    void*               mem;       \
//    pthread_rwlock_t    lock;      \
//    type                elem;      \
//}
//
//int bs_vector_lock_init(_vector_t* vector, uint32_t size, uint32_t esize, bool_t islock);
//int bs_vector_lock_add(_vector_t* vector, void* elem);
//void* bs_vector_lock_getback(_vector_t* v);
//
//#define vector_esize(vector)    ((vector)->esize)
//#define vector_size(vector)     ((vector)->size)
//#define vector_vlen(vector)     ((vector)==NULL ? 0:(vector)->vlen)
//#define vector_count(v)         ((v)==NULL ? 0:(v)->vlen)
//#define vector_clear(v)         ((v)->vlen = 0)
//    
//#define vector_init(v)              bs_vector_init((_vector_t*)(v), VECTOR_DEF_SIZE, sizeof((v)->elem), BS_FALSE)
//#define vector_init_lock(v)         bs_vector_init((_vector_t*)(v), VECTOR_DEF_SIZE, sizeof((v)->elem), BS_TRUE)
//#define vector_init_size(v, size)   bs_vector_init((_vector_t*)(v), size, sizeof((v)->elem), BS_FALSE)
//#define vector_init_size_lock(v, size)  bs_vector_init((_vector_t*)(v), size, sizeof((v)->elem), BS_TRUE)
//#define vector_push(v, elem)        bs_vector_add((_vector_t*)(v), elem)
//#define vector_get(v, i)            (((i)*vector_esize(v)>=(v)->size) ? NULL:((char*)(v)->mem + (i)*vector_esize(v)))
//#define vector_position(v, p)       (((uint32_t)((char*)(p)-(char*)(v)->mem))/vector_esize(v))
//#define vector_destroy(v)           free((v)->mem)
//#define vector_getback(v)           bs_vector_getback((_vector_t*)(v))
//// 不考虑越界
//#define vector_index(v, i)          ((char*)(v)->mem + vector_esize(v)*i)
///*
//#define vector_init_size(v, len)   do{              \
//    assert((v)!=NULL);                              \
//    (v)->vlen = 0;                                  \
//    (v)->size = len;                                \
//    (v)->esize = vector_esize(v);                   \
//    (v)->mem = malloc(vector_esize(v)*(v)->size);   \
//    (v)->st = (v)==NULL ? BS_NOMEM:BS_SUCCESS;      \
//}while(0)
//*/

#ifdef __cplusplus
}
#endif
#endif
