/**
 * file :	bs_object.h
 * author :	bushaofeng
 * create :	2016-08-25 13:22
 * func : 	c语言实现面向对象
 * history:
 */

#ifndef	__BS_OBJECT_H_
#define	__BS_OBJECT_H_

#include "bs_type.h"
#include "bs_error.h"
#ifdef __cplusplus
extern "C"{
#endif

#define OBJECT_NAME_SIZE	64

typedef struct object_t{
	uint32_t	size;
	char		name[OBJECT_NAME_SIZE];
	// 构造函数
	void* (*constructor)(void*);
	// 析构函数
	void (*destructor)(void*);
}object_t;

/// 创建一个对象
void* new_object(const char* name, uint32_t esize, void* (*constructor)(void*), void (*destructor)(void*));

/// 创建一个对象
#define bs_new(type)    ((type##_t*)new_object(#type, sizeof(type##_t), type##_init, type##_destroy))
/// 销毁对象
#define bs_delete(o)	do{	\
	((object_t*)(o))->destructor(o);	\
	free(o);			\
}while(0)

/// 对象类型名 
#define typename(o)	((object_t*)(o))->name

#ifdef __cplusplus
}
#endif
#endif
