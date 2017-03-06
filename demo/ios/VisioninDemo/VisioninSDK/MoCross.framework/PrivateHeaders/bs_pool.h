/**
 * pool:池
 * bushaofeng. 2012.1.13
 * history:
 *              2013.12.18 buffer改为使用vector
 */

#ifndef __BS_POOL_H_
#define __BS_POOL_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_vector.h"
#include "bs_object.h"

#ifdef __cplusplus
extern "C"{
#endif

#define POOL_DEF_SIZE   1024

typedef struct _pool_head_t{
    struct _pool_head_t*    next;
}_pool_head_t;

typedef struct _pool_t{
    object_t            base;
    uint32_t            used_cnt;
    _pool_head_t*       unused;
    _vector_t           buf;
}_pool_t;

#define pool_t(type)    struct{         \
    object_t            base;           \
    uint32_t            used_cnt;       \
    _pool_head_t*       unused;         \
    vector_t(type)      buf;            \
}

#define pool_init(pool)             pool_init_size((pool), POOL_DEF_SIZE, BS_FALSE)
#define pool_init_lock(pool,lock)   pool_init_size((pool), POOL_DEF_SIZE, BS_TRUE)
#define pool_init_size(pool, size, lock)    bs_pool_init((_pool_t*)(pool), (size), sizeof((pool)->buf.elem), (lock))
#define pool_malloc(pool)           bs_pool_malloc((_pool_t*)(pool))
#define pool_free(pool, mem)        bs_pool_free((_pool_t*)(pool), (mem))
#define pool_index(pool, i)         (vector_get(&(pool)->buf, i)==NULL ? NULL: ((char*)vector_get(&(pool)->buf, i)+sizeof(_pool_head_t)))
#define pool_position(pool, p)      vector_position(&(pool)->buf, (char*)(p)-sizeof(_pool_head_t))
#define pool_destroy(pool)          vector_destroy(&(pool)->buf)

state_t bs_pool_init(_pool_t* pool, uint32_t size, uint32_t esize, bool_t islock);
void* bs_pool_malloc(_pool_t* pool);
void bs_pool_free(_pool_t* pool, void* mem);

#ifdef __cplusplus
}
#endif
#endif
