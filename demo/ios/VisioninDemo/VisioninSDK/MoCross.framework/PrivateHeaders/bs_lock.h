/**
 * file :	bs_lock.h
 * author :	bushaofeng
 * create :	2013-11-19 23:00
 * func : 
 */

#ifndef	__BS_LOCK_H_
#define	__BS_LOCK_H_

#include "bs_type.h"
#include "bs_error.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef int bs_lock_t;

#define SPINLOCK_INIT(lock)	((lock)=0)
#define SPINLOCK_LOCK(lock)	while(++lock>1){	\
	--lock;	\
}
#define SPINLOCK_TRYLOCK(lock)	(++lock==1 ? BS_SUCCESS:(--lock))
#define SPINLOCK_UNLOCK(lock)	(--lock)

void bs_lock_init(bs_lock_t* lock);
void bs_rdlock(bs_lock_t* lock);
state_t bs_rdlock_try(bs_lock_t* lock);
state_t bs_rdlock_unlock(bs_lock_t* lock);
    
void bs_wrlock(bs_lock_t* lock);
state_t bs_wrlock_try(bs_lock_t* lock);
void bs_wrlock_unlock(bs_lock_t* lock);


#ifdef __cplusplus
}
#endif
#endif
