/**
 * file :	bs_cqueue.h
 * author :	bushaofeng
 * create :	2014-01-22 17:23
 * func :   循环队列
 * history:
 */

#ifndef	__BS_CQUEUE_H_
#define	__BS_CQUEUE_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_lock.h"

#ifdef __cplusplus
extern "C"{
#endif

#define CQUEUE_DEF_SIZE     1024

typedef struct _cqueue_t{
    uint8_t             islock:1;
    uint32_t            esize:31;
    uint32_t            size;
    uint32_t            head;
    uint32_t            rear;
    bs_lock_t           lock;
    void*               mem;
}_cqueue_t;

// 实际提供csize-1个空间，head指向第一个使用的空间，rear指向第一个未用的空间
#define cqueue_t(type)  struct{         \
    uint8_t             islock:1;       \
    uint32_t            esize:31;       \
    uint32_t            size;           \
    uint32_t            head;           \
    uint32_t            rear;           \
    bs_lock_t           lock;           \
    void*               mem;            \
    type                elem;           \
}

#define cqueue_index(c, i)  ((i) & ((c)->size-1))
#define cqueue_next(c, i)   cqueue_index(c, i+1)
#define cqueue_prev(c, i)   cqueue_index(c, i-1)
#define cqueue_last(c)      ((c)->rear==0 ? ((c)->size-1):((c)->rear-1))
#define cqueue_size(c)      ((c)->size)
#define cqueue_length(c)    ((c)->rear>=(c)->head ? ((c)->rear-(c)->head):((c)->size-((c)->head-(c)->rear)))
#define cqueue_full(c)      ((c)->head == cqueue_next(c, (c)->rear))
#define cqueue_empty(c)     ((c)->head == (c)->rear)
#define cqueue_elen(c)      ((c)->esize)
#define cqueue_addr(c, i)   ((char*)(c)->mem + cqueue_elen(c)*i)

// head前移一个位置
#define cqueue_head_back(c) do{             \
    (c)->head = cqueue_prev(c, (c)->head);  \
}while(0)

#define cqueue_contain(c,i) ((!cqueue_empty(c)) && \
    (( (c)->head<(c)->rear && (i>=(c)->head && i<(c)->rear)) || \
    ( (c)->head>=(c)->rear && ((i>=(c)->head && i<(c)->size) || (i<(c)->rear)) ) ))

#define cqueue_set(c, i, e) memcpy(cqueue_addr(c, cqueue_index(c, i)), e, (c)->esize)
#define cqueue_clear(c)     ((c)->rear = (c)->head)
#define cqueue_destroy(c)   free((c)->mem);
    
#define cqueue_init(c)      bs_cqueue_init((_cqueue_t*)(c), CQUEUE_DEF_SIZE, sizeof((c)->elem), BS_FALSE)
#define cqueue_init_lock(c) bs_cqueue_init((_cqueue_t*)(c), CQUEUE_DEF_SIZE, sizeof((c)->elem), BS_TRUE)
#define cqueue_init_size(c, size, islock)   bs_cqueue_init((_cqueue_t*)(c), size, sizeof((c)->elem), islock)

#define cqueue_push(c, e)   bs_cqueue_push((_cqueue_t*)(c), e, (c)->esize)
#define cqueue_pop(c, e)    bs_cqueue_pop((_cqueue_t*)(c), e, (c)->esize)
#define cqueue_get(c, i)    bs_cqueue_get((_cqueue_t*)(c), i)
#define cqueue_get_any(c, i)    cqueue_addr(c, i)
#define cqueue_search(c,e,cmp)  bs_cqueue_search((_cqueue_t*)(c), e, cmp)

state_t bs_cqueue_init(_cqueue_t* c, uint32_t size, uint32_t esize, bool_t islock);
void* bs_cqueue_push(_cqueue_t* c, void* e, uint32_t size);
state_t bs_cqueue_pop(_cqueue_t* c, void* e, uint32_t size);
void* bs_cqueue_get(_cqueue_t* c, uint32_t i);
void* bs_cqueue_search(_cqueue_t* c, void* e, compare_f cmp);
void* bs_cqueue_set(_cqueue_t* c, void* e);

#ifdef __cplusplus
}
#endif
#endif
