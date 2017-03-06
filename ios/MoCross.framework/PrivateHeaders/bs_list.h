/**
 * file :	bs_list.h
 * author :	bushaofeng
 * create :	2013-12-20 07:16
 * func : 
 * history:
 */

#ifndef	__BS_LIST_H_
#define	__BS_LIST_H_

#include "bs_type.h"
#include "bs_def.h"
#include "bs_error.h"
#include "bs_pool.h"

#ifdef __cplusplus
extern "C"{
#endif

/* 动态双向链表 */
typedef struct _list_head_t{
    struct _list_head_t*    prev;
    struct _list_head_t*    next;
}_list_head_t;

typedef struct _list_t{
    bool_t                  islock:1;
    uint32_t                esize:31;
    uint32_t                size;
    _list_head_t            head;
    _list_head_t            rear;
    pthread_rwlock_t        lock;
    _pool_t                 pool;
}_list_t;

#define list_t(type)    struct{                     \
    bool_t                  islock:1;               \
    uint32_t                esize:31;               \
    uint32_t                size;                   \
    _list_head_t            head;                   \
    _list_head_t            rear;                   \
    pthread_rwlock_t        lock;                   \
    pool_t(type)            pool;                   \
}

#define LIST_DEF_SIZE           64

#define list_head(list)         (list_empty(list) ? NULL:((list)->head.next+1))
#define list_rear(list)         (list_empty(list) ? NULL:((list)->rear.prev+1))
#define list_size(list)         ((list)->size)
#define list_elen(list)         ((list)->esize)

#define list_addr(elem)         ((_list_head_t*)((char*)(elem)-sizeof(_list_head_t)))
#define list_next(elem)         ((char*)(list_addr(elem)->next)+sizeof(_list_head_t))
#define list_prev(elem)         ((char*)(list_addr(elem)->prev)+sizeof(_list_head_t))

#define list_forward(elem)      ((elem)!=NULL && list_addr(elem)->next != NULL)
#define list_back(elem)         ((elem)!=NULL && list_addr(elem)->prev != NULL)
#define list_empty(list)        ((list)->head.next==&(list)->rear)

#define list_init(list)                 _list_init((_list_t*)(list), LIST_DEF_SIZE, sizeof((list)->pool.buf.elem), BS_FALSE)
#define list_init_lock(list)            _list_init((_list_t*)(list), LIST_DEF_SIZE, sizeof((list)->pool.buf.elem), BS_TRUE)
#define list_init_size(list,size,lock)  _list_init((_list_t*)(list), (size), sizeof((list)->pool.buf.elem), (lock))

#define list_insert_rear(list, elem)    _list_insert_rear((_list_t*)(list), (elem))
#define list_insert_head(list, elem)    _list_insert_head((_list_t*)(list), (elem))
#define list_insert(list, elem)         _list_insert_rear((_list_t*)(list), (elem))
#define list_insert_next(list,cur,elem) _list_insert_next((_list_t*)(list), (cur), (elem))
#define list_insert_prev(list,cur,elem) _list_insert_prev((_list_t*)(list), (cur), (elem))

#define list_remove(list, elem)         _list_remove((_list_t*)(list), (elem))
#define list_clear(list)                _list_clear((_list_t*)(list));
#define list_destroy(list)              pool_destroy(&(list)->pool)

typedef int (* list_compare_f)(const void* e1, const void* e2);
#define list_search(list,data,cmp)      _list_search((_list_t*)(list), (data), (list_compare_f)(cmp))
#define list_set(list, key, data, cmp)  _list_set((_list_t*)(list), (key), (data), (list_compare_f)(cmp))
#define list_index(list, i)             _list_index((_list_t*)(list), (i))
// 超出list节点数时，以NULL补充
#define list_set_index(list, i, elem)   _list_set_index((_list_t*)(list), (i), (elem))
// i不是节点的序号，是内存的位置
#define list_position(list, i)          (pool_index(&(list)->pool, (i)) + sizeof(_list_head_t))

state_t _list_init(_list_t* list, uint32_t size, uint32_t esize, bool_t lock);
void* _list_insert_head(_list_t* list, void* elem);
void* _list_insert_rear(_list_t* list, void* elem);
void* _list_insert_next(_list_t* list, void* cur, void* elem);
void* _list_insert_prev(_list_t* list, void* cur, void* elem);

state_t _list_remove(_list_t* list, void* elem);
state_t _list_clear(_list_t* list);
void* _list_search(_list_t* list, const void* elem, list_compare_f compare);
void* _list_set(_list_t* list, const void* key, void* data, list_compare_f compare);
void* _list_index(_list_t* list, uint32_t i);
void* _list_set_index(_list_t* list, uint32_t idx, void* elem);

#ifdef __cplusplus
}
#endif
#endif
