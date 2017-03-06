/**
 * bushaofeng. 2011.8.25
 * bs_struct.h：基本数据结构定义
 * history:
 *              2013.12.1 list元素空间由内部申请改为外部传入
 */

#ifndef __BS_STRUCT_H_
#define __BS_STRUCT_H_

#include "bs_type.h"
#include "bs_def.h"
#include "bs_error.h"
#include "bs_pool.h"
#include "bs_macros.h"

#ifdef __cplusplus
extern "C"{
#endif

#define ARRAY_DEFAULT_NUM   64
/* 静态数组 */
#define array_num_t(type, num)   struct{    \
    type        e[num];                     \
    type        *elem;                      \
    int         size;                       \
    uint32_t    vlen;                       \
    bool_t      repeat;                     \
	int         ptr;                        \
	type* cast(void *p) {return (type*)p;}  \
}
#define array_t(type)   array_num_t(type, 1)

// 默认不允许重复
#define array_repeat(a)     ((a)->repeat = BS_TRUE)
#define array_esize(a)      (sizeof((a)->e[0]))
#define array_size(a)       ((a)->size)
#define array_count(a)      ((a)->vlen)
#define array_extended(a)   ((a)->elem != (a)->e)
#define array_ptr(a)        ((a)->ptr)
#define array_valid(a)      ((a)->ptr >= 0)

// 初始化，动态扩张，释放
#define array_init(a)       do{\
	memset((a), 0, sizeof(*(a)));\
	(a)->elem = (a)->e;\
	(a)->size = sizeof((a)->e)/array_esize(a);\
	(a)->ptr = -1;\
}while(0)

#define array_invalid(a)    do{\
	(a)->vlen = 0;\
	(a)->ptr = -1;\
}while(0)

#define array_clear(a)      do{\
	if(array_extended(a)) {\
		char *p_ = (char*)((a)->elem);\
		delete p_;\
	}\
	array_init(a);\
}while(0)

#define array_access(a, i)  do{\
	int i_= ((a)->ptr=(i))+1;\
	(a)->vlen = MAX((a)->vlen,i_);\
	i_-= (a)->size;\
	if(i_> 0) {\
		int psize_ = (a)->size*array_esize(a);\
		int m_= MIN((a)->size,ARRAY_DEFAULT_NUM);\
		(a)->size += MAX(i_,m_);\
		int qsize_ = (a)->size*array_esize(a);\
		char *p_= (char*)((a)->elem);\
		char *q_= new char[qsize_];\
		for(i_= 0; i_< psize_; i_++) q_[i_] = p_[i_];\
		if(array_extended(a)) delete p_;\
		(a)->elem = (a)->cast(q_);\
	}\
}while(0)

// 只查找第一个匹配元素
#define array_index(a, e)   do{\
	(a)->ptr = -1;\
	for(int i_= 0; i_< (a)->vlen; i_++) {\
		if((a)->elem[i_]==(e)) {\
			(a)->ptr = i_; break;\
		}\
	}\
}while(0)

#define array_find(a, e, idx)   do{\
	array_index(a, e);\
	(idx) = (a)->ptr;\
}while(0)
    
// 不检查重复
#define array_add_repeat(a, e)   do{\
	array_access(a, (a)->vlen);\
	(a)->elem[(a)->ptr] = (e);\
}while(0)

// 检查重复元素，如果有则不加入
#define array_add(a, e) do{ \
	if((a)->repeat) array_add_repeat(a, e);\
	else{\
		array_index(a, e);\
		if(!array_valid(a)) array_add_repeat(a, e);\
	}\
}while(0)

#define array_get(a, i)		((a)->elem[(a)->ptr = (i)])
#define array_set(a, i, e)	do{\
	array_access(a, i);\
	if(array_valid(a)) (a)->elem[(a)->ptr] = (e);\
}while(0)

#define array_remove(a, idx)	do{\
	(a)->ptr=(idx);\
	if((a)->ptr >= (a)->vlen) (a)->ptr = -1;\
	else if((a)->ptr >= 0) {\
		for(int i_=(a)->ptr+1; i_< (a)->vlen; i_++) (a)->elem[i_-1] = (a)->elem[i_];\
		(a)->vlen --;\
    }\
}while(0)

// 查找并删除某个元素，如果有多个只会删除第一个
#define array_removeitem(a, e)	do{\
	array_index(a, e);\
	array_remove(a, (a)->ptr);\
}while(0)






/* 静态链表 */
#define ring_t(type, size)      struct{ \
}

/* hash */
typedef struct _bs_hash_t      _bs_hash_t;
typedef _bs_hash_t               hash_t;
typedef struct _bs_hash_node_t _bs_hash_node_t;
typedef int (* _bs_hash_f)(char* str, int len);

#define BYTE_BITS               8
struct _bs_hash_t{
    int                 size;
    _bs_hash_node_t*    hash;
};

struct _bs_hash_node_t{
    uint32_t            hsign;
    uint32_t            lhigh;
    int                 code;
    _bs_hash_node_t*    next;
};
    
/* pair */
#define pair_t(type1, type2) struct{    \
    type1   first;  \
    type2   second; \
}
#define pair_init(p, f, s)  do{ \
    (p)->first = (f); (p)->second = (s);    \
}while(0)


#ifdef __cplusplus
}
#endif
#endif
